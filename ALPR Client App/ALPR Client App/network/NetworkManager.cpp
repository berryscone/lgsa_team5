#include "NetworkManager.h"
#include "model/VehicleInfoModel.h"

#include <QThread>

#define USE_LOCALHOST_TEST

NetworkManager::NetworkManager(QObject *parent)
{
    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
}

NetworkManager::~NetworkManager()
{
    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
}

void NetworkManager::OnStart()
{
    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    mManager = std::make_unique<QNetworkAccessManager>();
}

void NetworkManager::OnStop()
{
    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    emit Finished();
}

void NetworkManager::OnRequestLogin(QString url, QString id, QString pw)
{
    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

#ifdef USE_LOCALHOST_TEST
    QUrl login_url("http://localhost");

    QNetworkRequest request(login_url);
    mReply.reset(mManager->get(request));
#else
    //TODO : 서버에 로그인 요청
    QUrl login_url(url);
    login_url.setPath("/login/");

    QString auth = id + ":" + pw;
    QByteArray auth_base64 = auth.toLocal8Bit().toBase64();
    QString basic_auth = "Basic " + auth_base64;

    QNetworkRequest request(login_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", basic_auth.toLocal8Bit());

    QByteArray data;
    mReply.reset(mManager->post(request, data));
#endif

    connect(mReply.get(), &QNetworkReply::finished, this, &NetworkManager::OnLoginFinished);
    connect(mReply.get(), &QNetworkReply::sslErrors, this, &NetworkManager::OnSslErrors);
    connect(mReply.get(), &QIODevice::readyRead, this, &NetworkManager::OnLoginReadReady);
}

void NetworkManager::OnRequestQuery(QString url, QString licensePlate)
{
    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

#ifdef USE_LOCALHOST_TEST
    QUrl query_url("http://localhost");

    QNetworkRequest request(query_url);
#else
    //TODO : 서버에 번호판 쿼리 요청
    QUrlQuery query;
    query.addQueryItem("license-plate-number", licensePlate);

    QUrl query_url(url);
    query_url.setQuery(query.query());

    QString token_auth = "Token " + mToken; //TODO : LoginModel로 부터 token을 가져오도록 수정 필요

    QNetworkRequest request(query_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", token_auth.toLocal8Bit());
#endif

    mReply.reset(mManager->get(request));

    connect(mReply.get(), &QNetworkReply::finished, this, &NetworkManager::OnQueryFinished);
    connect(mReply.get(), &QIODevice::readyRead, this, &NetworkManager::OnQueryReadReady);
}

void NetworkManager::OnLoginReadReady()
{
#ifdef USE_LOCALHOST_TEST
    QString answer = mReply->readAll();

    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    //qDebug() << "managerReadReady:" << answer;

    OnStop();
#else
    //TODO : 서버에서 인증된 경우 ID, Token 정보를 LoginModel에 업데이트
    QByteArray data = mReply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    mToken = obj["token"].toString();

    qDebug() << "Token : " << mToken;

    //TODO : update LoginModel (id : token) pair 조합
#endif
}

void NetworkManager::OnLoginFinished()
{

}

void NetworkManager::OnQueryReadReady()
{
#ifdef USE_LOCALHOST_TEST
    QString answer = mReply->readAll();

    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId() << ", server query delay test. wait 1 sec... in here";
    //qDebug() << "managerReadReady:" << answer;

    static unsigned int plateCount = 0;
    plateCount++;
    std::string vehicleInfo = "NetworkManager DEMO PlateCount : " + std::to_string(plateCount) + "\n" + answer.toStdString();
    VehicleInfoModel::GetInstance().SetVehicleInfoData(vehicleInfo);

    OnStop();
#else
    //TODO : 서버에서 번호판 쿼리 결과오면 VehicleInfo 정보를 VehicleInfoModel에 업데이트
    //       쿼리 결과는 하나만 온다고 가정해도 되려나?
    QByteArray data = mReply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QString json = doc.toJson(QJsonDocument::Indented);

    qDebug() << "Query result : " << json;

    //TODO : Update VehicleInfoModel
#endif
}

void NetworkManager::OnQueryFinished()
{

}

void NetworkManager::OnSslErrors(const QList<QSslError> &errors)
{

}