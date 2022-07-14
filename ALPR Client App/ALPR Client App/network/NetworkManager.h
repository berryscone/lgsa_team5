#pragma once
#include <memory>

#include <QDebug>
#include <QObject>
#include <QNetworkAccessManager>
#include <QtNetwork>

#include "NetworkInterfaces.h"


class NetworkManager :
    public IVehicleQueryProvider,
    public ILoginProvider
{
    Q_OBJECT

public:
    static NetworkManager& GetInstance();
    virtual ~NetworkManager() = default;
    virtual void RequestLogin(const QString id, const QString pw, LoginCallback callback);

signals:
    void SignalLoginCallback(const bool success, const QString detail);
    void SignalVehicleDetailProvide(const cv::Mat plate_image, const QJsonObject vehicle_detail);

public slots:
    virtual void RequestVehicleQuery(const cv::Mat plate_image, const QString plate_number);

private:
    NetworkManager();

    void OnLoginReadReady(LoginCallback callback, QNetworkReply* reply);
    void OnLoginError(QNetworkReply::NetworkError error, QNetworkReply* reply);

    void OnVehicleQueryReadReady(const cv::Mat plate_image, const QString plate_number, QNetworkReply* reply);
    void OnVehicleQueryError(QNetworkReply::NetworkError error, QNetworkReply* reply);

    void OnSslErrors(const QList<QSslError>& errors, QNetworkReply* reply);

    QThread mThread;
    QNetworkAccessManager mManager;
    QUrl mUrl;
    QString mToken;
};
