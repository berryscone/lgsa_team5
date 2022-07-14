#pragma once

#include <mutex>
#include <vector>

#include <QVector>
#include <QRect>
#include <QImage>

#include <opencv2/opencv.hpp>

class IRecentPlatesListener {
public:
    virtual ~IRecentPlatesListener() = default;
    virtual void OnRecentPlatesUpdated(const QImage &licensePlateImage, const QString &vehicleInfo) = 0;
};

class RecentPlatesModel
{
public:
    static RecentPlatesModel& GetInstance();
    void ResetData();
    void ResetListener();

    void SubscribeListener(IRecentPlatesListener *pListener);
    void UnSubscribeListener(IRecentPlatesListener *pListener);

    void SetRecentPlatesData(const QImage &licensePlateImage, const QString &vehicleInfo);
    QImage GetRecentPlateImage();
    QString GetVehicleInfo();

private:
    RecentPlatesModel();

    void NotifyDataChanged(const QImage &licensePlateImage, const QString &vehicleInfo);

    std::mutex mMutex;
    std::mutex mListenerVectorMutex;

    std::vector<IRecentPlatesListener*> mListenerVector;

    QImage mLicensePlateImage;
    QString mVehicleInfo;
};
