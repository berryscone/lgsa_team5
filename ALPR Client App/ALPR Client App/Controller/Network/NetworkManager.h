#pragma once
#include <memory>

#include <opencv2/opencv.hpp>

#include <QtCore>
#include <QtNetwork>

#include "VehicleCache.h"


using LoginCallback = std::function<void(const bool success, const QString detail)>;

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    static NetworkManager& GetInstance();
    void RequestLogin(const QString id, const QString pw, LoginCallback callback);

signals:
    void SignalLoginCallback(const bool success, const QString detail);
    void SignalVehicleDetailProvide(const cv::Mat plate_image, const QJsonObject vehicle_detail);
    void SignalNetworkStatusChanged(QNetworkReply::NetworkError status);
    void SignalRetryVehicleDetail(const cv::Mat plate_image, const QString plate_number, const int retry_cnt);

public slots:
    void RequestVehicleQuery(const cv::Mat plate_image, const QString plate_number, const int retry_cnt = 0);

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

    const int mStatusTimeoutMs = 1500;
    const int mRequestTimeoutMs = 1500;
    std::unique_ptr<QTimer> mStatusTimer;   // QTimer create, start, stop must be done in the same thread
    QNetworkReply::NetworkError mCurrentStatus = QNetworkReply::NetworkError::NoError;
    
    VehicleCache mCache;
};
