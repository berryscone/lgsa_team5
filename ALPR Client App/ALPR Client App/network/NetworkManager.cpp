#include "NetworkManager.h"
#include "model/VehicleInfoModel.h"

#include <QThread>


NetworkManager& NetworkManager::GetInstance()
{
    static NetworkManager instance;
    return instance;
}

NetworkManager::NetworkManager()
{
    mUrl.setUrl("https://peaceful-atoll-24696.herokuapp.com/");
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

    connect(reply, &QNetworkReply::sslErrors, 
        this, [this, reply](const QList<QSslError>& errors) { this->OnSslErrors(errors, reply); });

    connect(reply, &QIODevice::readyRead, this, 
        [this, reply, callback]() { this->OnLoginReadReady(callback, reply); });

    connect(reply, &QNetworkReply::errorOccurred, 
        this, [this, reply](QNetworkReply::NetworkError error) { this->OnLoginError(error, reply); });
}

void NetworkManager::OnLoginReadReady(LoginCallback callback, QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QVariant status_code_var = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    const int status_code = status_code_var.toInt();

    if (status_code == 200) {
        mToken = obj["token"].toString();
        qDebug() << "Authentication Token: " << mToken;
        callback(true, "");

        this->moveToThread(&mThread);
        mManager.moveToThread(&mThread);
        mThread.start();
    }
    else {
        const QString detail = obj["detail"].toString();
        qDebug() << "Authentication Failed: " << detail;
        callback(false, detail);
    }
}

void NetworkManager::OnLoginError(QNetworkReply::NetworkError error, QNetworkReply* reply)
{
    // TODO: handle error
}

void NetworkManager::RequestVehicleQuery(const cv::Mat plate_image, const QString plate_number)
{
    QUrlQuery query;
    query.addQueryItem("license-plate-number", plate_number);

    QUrl query_url(mUrl);
    query_url.setQuery(query.query());

    QString token_auth = "Token " + mToken;

    QNetworkRequest request(query_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", token_auth.toLocal8Bit());
    QNetworkReply* reply = mManager.get(request);

    connect(reply, &QIODevice::readyRead,
        this, [this, plate_image, plate_number, reply]() { this->OnVehicleQueryReadReady(plate_image, plate_number, reply); });

    connect(reply, &QNetworkReply::errorOccurred,
        this, [this, reply](QNetworkReply::NetworkError error) { this->OnVehicleQueryError(error, reply); });

    connect(reply, &QNetworkReply::sslErrors,
        this, [this, reply](const QList<QSslError>& errors) { this->OnSslErrors(errors, reply); });
}

void NetworkManager::OnVehicleQueryReadReady(const cv::Mat plate_image, const QString plate_number, QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QVariant status_code_var = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    const int status_code = status_code_var.toInt();

    // TODO: send result to appropriate handler
}

void NetworkManager::OnVehicleQueryError(QNetworkReply::NetworkError error, QNetworkReply* reply)
{
    // TODO: handle error
}

void NetworkManager::OnSslErrors(const QList<QSslError> &errors, QNetworkReply* reply)
{
    reply->ignoreSslErrors();
}
