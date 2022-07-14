#include <QThread>

#include "NetworkManager.h"
#include "handler/VehicleDetailHandler.h"


// TODO: 캐시 사용 유무 정하기. 활성화 할 거면 내부 qDebug() 코드 다 지우기
//#define USE_VEHICLE_QUERY_CACHE

NetworkManager& NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

NetworkManager::NetworkManager()
{
    // TODO: 데모용 배포할 때 URL 바꿀 것 또는 외부에서 입력 가능하게 export
    mUrl.setUrl("https://peaceful-atoll-24696.herokuapp.com/");
    // mUrl.setUrl("http://localhost:8000/");

    connect(this, &NetworkManager::SignalVehicleDetailProvide,
        &VehicleDetailHandler::GetInstance(), &VehicleDetailHandler::OnVehicleDetailProvided,
        Qt::QueuedConnection);

    connect(this, &NetworkManager::SignalRetryVehicleDetail,
        this, &NetworkManager::RequestVehicleQuery,
        Qt::QueuedConnection);

    connect(&mThread, &QThread::started, 
        this, &NetworkManager::StartStatusTimer, 
        Qt::QueuedConnection);
}

void NetworkManager::RequestLogin(const QString id, const QString pw, LoginCallback callback)
{
    QUrl login_url(mUrl);
    login_url.setPath("/login/");

    QString auth = id + ":" + pw;
    QByteArray auth_base64 = auth.toLocal8Bit().toBase64();
    QString basic_auth = "Basic " + auth_base64;

    QNetworkRequest request(login_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", basic_auth.toLocal8Bit());

    QByteArray data;
    // TODO: reply 메모리 관리 따로 안해도 되는지 확인
    QNetworkReply* reply = mManager.post(request, data);

    connect(reply, &QNetworkReply::finished, this,
        [this, reply, callback]() { this->OnLoginFinished(callback, reply); });

    connect(reply, &QNetworkReply::sslErrors,
        this, [this, reply](const QList<QSslError>& errors) { this->OnSslErrors(errors, reply); });
}


void NetworkManager::RequestVehicleQuery(const cv::Mat plate_image, const QString plate_number, const int retry_cnt)
{
#ifdef USE_VEHICLE_QUERY_CACHE
    if (mCache.isRequestedQuery(plate_number)) {
        qDebug() << "Already Requested for " << plate_number;
        QJsonObject obj = mCache.getQueryResult(plate_number);
        if (obj.isEmpty()) {
            qDebug() << "[Progress..] Query is in progress. Cache is empty!!";
        } else {
            qDebug() << "[Finish!!] Query was finished. Use Cache data!!";
            qDebug() << "Cache data: " << obj;
            emit SignalVehicleDetailProvide(plate_image, obj);
        }
        return;
    } else {
        mCache.addRequestedList(plate_number);
        qDebug() << "New query start for " << plate_number;
    }
#endif

    QUrlQuery query;
    query.addQueryItem("license-plate-number", plate_number);

    QUrl query_url(mUrl);
    query_url.setQuery(query.query());

    QString token_auth = "Token " + mToken;

    QNetworkRequest request(query_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", token_auth.toLocal8Bit());
    QNetworkReply* reply = mManager.get(request);

    connect(reply, &QNetworkReply::finished,
        this, [this, plate_image, plate_number, reply, retry_cnt]() { this->OnVehicleQueryFinished(plate_image, plate_number, reply, retry_cnt); });

    connect(reply, &QNetworkReply::sslErrors,
        this, [this, reply](const QList<QSslError>& errors) { this->OnSslErrors(errors, reply); });
}

void NetworkManager::RequestHealthCheck()
{
    QUrl health_url(mUrl);
    health_url.setPath("/health/");

    QString token_auth = "Token " + mToken;

    QNetworkRequest request(health_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", token_auth.toLocal8Bit());
    QNetworkReply* reply = mManager.get(request);

    connect(reply, &QNetworkReply::finished,
        this, [this, reply]() { this->OnHealthCheckFinished(reply); });

    connect(reply, &QNetworkReply::errorOccurred,
        this, &NetworkManager::UpdateStatus);

    connect(reply, &QNetworkReply::sslErrors,
        this, [this, reply](const QList<QSslError>& errors) { this->OnSslErrors(errors, reply); });
}


void NetworkManager::OnLoginFinished(LoginCallback callback, QNetworkReply* reply)
{
    const QNetworkReply::NetworkError error = reply->error();
    const int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (status_code == 200) {
        mToken = obj["token"].toString();
        qDebug() << __FUNCTION__ << " - Authentication Token: " << mToken;
        callback(true, "");

        this->moveToThread(&mThread);
        mManager.moveToThread(&mThread);
        mThread.start();
    }
    else if (error == QNetworkReply::NetworkError::AuthenticationRequiredError) {
        // TODO: 에러 원인 문자열 변경
        const QString detail = obj["detail"].toString();
        qDebug() << __FUNCTION__ << " - Authentication Fail: " << detail;
        callback(false, detail);
    }
    else {
        // TODO: 에러 원인 문자열 변경
        const QString detail = reply->errorString();
        qDebug() << __FUNCTION__ << " - Authentication Error: " << detail;
        callback(false, detail);
    }
}

void NetworkManager::OnVehicleQueryFinished(const cv::Mat plate_image, const QString plate_number, QNetworkReply* reply, const int retry_cnt)
{
    ResetStatusTimer();

    const QNetworkReply::NetworkError error = reply->error();
    const int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (error == QNetworkReply::NetworkError::NoError && status_code == 200) {
        QByteArray data = reply->readAll();
        QJsonObject json_data = QJsonDocument::fromJson(data).object();
        
#ifdef USE_VEHICLE_QUERY_CACHE
        mCache.putQueryResult(plate_number, json_data);
#endif
        emit SignalVehicleDetailProvide(plate_image, json_data);
    }
    else if (retry_cnt <= mMaxRetry) {
        qDebug() << __FUNCTION__ << " : retry query (" << retry_cnt << ")";
        emit SignalRetryVehicleDetail(plate_image, plate_number, retry_cnt + 1);
    }
    else {
        UpdateStatus(error);
    }
}

void NetworkManager::OnHealthCheckFinished(QNetworkReply* reply)
{
    const QNetworkReply::NetworkError error = reply->error();
    const int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QNetworkReply::NetworkError newStatus = mCurrentStatus;
    if (status_code == 200) {
        newStatus = QNetworkReply::NetworkError::NoError;
    }
    else if (status_code == 500) {
        newStatus = QNetworkReply::NetworkError::InternalServerError;
    }
    else {
        newStatus = error;
    }

    UpdateStatus(newStatus);
}

void NetworkManager::OnSslErrors(const QList<QSslError> &errors, QNetworkReply* reply)
{
    reply->ignoreSslErrors();
}

void NetworkManager::StartStatusTimer()
{
    if (!mStatusTimer) {
        mStatusTimer = std::unique_ptr<QTimer>(new QTimer);
        mStatusTimer->callOnTimeout(this, &NetworkManager::OnStatusTimeout);
        mStatusTimer->setSingleShot(true);
        qDebug() << "Create Network Status Timer";
    }
    mStatusTimer->start(mStatusTimeoutMs);
    qDebug() << "Start Network Status Timer";
}

void NetworkManager::ResetStatusTimer()
{
    if (mStatusTimer && mStatusTimer->isActive()) {
        mStatusTimer->setInterval(mStatusTimeoutMs);
    }
}

void NetworkManager::OnStatusTimeout()
{
    qDebug() << "Expire Network Status Timer";
    RequestHealthCheck();
    StartStatusTimer();
}

void NetworkManager::UpdateStatus(const QNetworkReply::NetworkError status)
{
    if (mCurrentStatus != status) {
        emit SignalNetworkStatusChanged(status);
        qDebug() << __FUNCTION__ << " : " << "network status changed from " << mCurrentStatus << " to " << status;
        mCurrentStatus = status;
    }
}
