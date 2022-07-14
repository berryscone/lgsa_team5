#pragma once
#include <memory>

#include <QDebug>
#include <QObject>
#include <QNetworkAccessManager>
#include <QtNetwork>

#include "NetworkInterfaces.h"
#include <network/VehicleCache.h>


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
    void SignalNetworkStatusChanged(QNetworkReply::NetworkError status);
    void SignalRetryVehicleDetail(const cv::Mat plate_image, const QString plate_number, const int retry_cnt);

public slots:
    virtual void RequestVehicleQuery(const cv::Mat plate_image, const QString plate_number, const int retry_cnt = 0);

private slots:
    void StartStatusTimer();
    void OnStatusTimeout();

private:
    NetworkManager();
    void ResetStatusTimer();
    void RequestHealthCheck();

    void OnLoginFinished(LoginCallback callback, QNetworkReply* reply);
    void OnVehicleQueryFinished(const cv::Mat plate_image, const QString plate_number, QNetworkReply* reply, const int retry_cnt = 0);
    void OnHealthCheckFinished(QNetworkReply* reply);

    void OnSslErrors(const QList<QSslError>& errors, QNetworkReply* reply);
    void UpdateStatus(const QNetworkReply::NetworkError status);


    QThread mThread;
    QNetworkAccessManager mManager;
    QUrl mUrl;
    QString mToken;

    const int mStatusTimeoutMs = 3000;
    const int mMaxRetry = 3;
    std::unique_ptr<QTimer> mStatusTimer;   // QTimer create, start, stop must be done in the same thread
    QNetworkReply::NetworkError mCurrentStatus = QNetworkReply::NetworkError::NoError;
    
    VehicleCache mCache;
};
