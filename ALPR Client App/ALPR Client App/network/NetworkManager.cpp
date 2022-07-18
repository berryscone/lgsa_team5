#include <QThread>

#include "NetworkManager.h"
#include "handler/VehicleDetailHandler.h"

NetworkManager& NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

NetworkManager::NetworkManager()
{
    // Read setting file
    QSettings settings("Alpr.ini", QSettings::IniFormat);
    QString url = settings.value("url").toString();
    if (url.trimmed().isEmpty()) {
        qDebug() << "No url provided";
        url = "https://peaceful-atoll-24696.herokuapp.com/";
    }
    mUrl.setUrl(url);
    qDebug() << "URL: " << url;

    connect(this, &NetworkManager::SignalVehicleDetailProvide,
        &VehicleDetailHandler::GetInstance(), &VehicleDetailHandler::OnVehicleDetailProvided,
        Qt::QueuedConnection);

    connect(this, &NetworkManager::SignalRetryVehicleDetail,
        this, &NetworkManager::RequestVehicleQuery,
        Qt::QueuedConnection);

    connect(&mThread, &QThread::started, 
        this, &NetworkManager::StartStatusTimer, 
        Qt::QueuedConnection);

    mManager.setTransferTimeout(mRequestTimeoutMs);
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
    if (mCache.isRequestedQuery(plate_number)) {
        QJsonObject obj = mCache.getQueryResult(plate_number);
        if (!obj.isEmpty()) {
            emit SignalVehicleDetailProvide(plate_image, obj);
        }
        return;
    } else {
        mCache.addRequestedList(plate_number);
    }

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
        qDebug() << "Login Success";
        mToken = obj["token"].toString();
        callback(true, "");

        this->moveToThread(&mThread);
        mManager.moveToThread(&mThread);
        mThread.start();
    }
    else if (error == QNetworkReply::NetworkError::AuthenticationRequiredError) {
        const QString detail = obj["detail"].toString();
        qDebug() << "Login Fail: " << detail;
        callback(false, detail);
    }
    else {
        const QString detail = "No server response";
        qDebug() << "Login Fail: " << detail;
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
        QJsonArray vehicleDetailArray = json_data["vehicle_details"].toArray();
        
        mCache.putQueryResult(plate_number, json_data);

        qDebug() << "Server Response: " << plate_number << " => " << vehicleDetailArray.size();
        emit SignalVehicleDetailProvide(plate_image, json_data);
    }
    else {
        qDebug() << "Request Failed: " << plate_number << " => " << error;
    }

    UpdateStatus(error);
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

    qDebug() << "Health Check - " << newStatus;
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
}

void NetworkManager::ResetStatusTimer()
{
    if (mStatusTimer && mStatusTimer->isActive()) {
        mStatusTimer->setInterval(mStatusTimeoutMs);
    }
}

void NetworkManager::OnStatusTimeout()
{
    RequestHealthCheck();
    StartStatusTimer();
}

void NetworkManager::UpdateStatus(const QNetworkReply::NetworkError status)
{
    if (mCurrentStatus != status) {
        mCurrentStatus = status;
        emit SignalNetworkStatusChanged(status);
        qDebug() << "network status changed: " << mCurrentStatus << " => " << status;
    }
}
