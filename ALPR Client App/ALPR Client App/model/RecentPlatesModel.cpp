#include "RecentPlatesModel.h"

#include <QDebug>

RecentPlatesModel &RecentPlatesModel::GetInstance()
{
    static RecentPlatesModel instance;
    return instance;
}

RecentPlatesModel::RecentPlatesModel()
{
    ResetData();
    ResetListener();
}

void RecentPlatesModel::ResetData()
{

}

void RecentPlatesModel::ResetListener()
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.clear();
    ResetData();
}

void RecentPlatesModel::SubscribeListener(IRecentPlatesListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.push_back(pListener);
}

void RecentPlatesModel::UnSubscribeListener(IRecentPlatesListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.erase(
                std::remove(mListenerVector.begin(),
                            mListenerVector.end(), pListener),
                            mListenerVector.end());
}

void RecentPlatesModel::SetRecentPlatesData(const QImage &licensePlateImage, const QString &vehicleInfo)
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mLicensePlateImage = licensePlateImage;
        mVehicleInfo = vehicleInfo;
    }
    NotifyDataChanged(mLicensePlateImage, mVehicleInfo);
}

QImage RecentPlatesModel::GetRecentPlateImage()
{
    return mLicensePlateImage;
}

QString RecentPlatesModel::GetVehicleInfo()
{
    return mVehicleInfo;
}

void RecentPlatesModel::NotifyDataChanged(const QImage &licensePlateImage, const QString &vehicleInfo)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    if (mListenerVector.size() <= 0) {
        qDebug() << "No listener!";
        ResetData();
        return;
    }

    for (const auto &listener : mListenerVector) {
        listener->OnRecentPlatesUpdated(licensePlateImage, vehicleInfo);
    }
}
