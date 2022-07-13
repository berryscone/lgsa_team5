#include "VehicleInfoModel.h"

#include <QDebug>


VehicleInfoModel &VehicleInfoModel::GetInstance()
{
    static VehicleInfoModel instance;
    return instance;
}

VehicleInfoModel::VehicleInfoModel()
{
    ResetData();
    ResetListener();
}

void VehicleInfoModel::ResetData()
{

}

void VehicleInfoModel::ResetListener()
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.clear();
    ResetData();
}

void VehicleInfoModel::SubscribeListener(IVehicleInfoListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.push_back(pListener);
}

void VehicleInfoModel::UnSubscribeListener(IVehicleInfoListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.erase(
                std::remove(mListenerVector.begin(),
                            mListenerVector.end(), pListener),
                            mListenerVector.end());
}

void VehicleInfoModel::SetVehicleInfoData(std::string &info)
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mVehicleInfo = info;
    }
    NotifyDataChanged(info);
}

std::string VehicleInfoModel::GetVehicleInfoData()
{
    std::unique_lock<std::mutex> lock(mMutex);
    return mVehicleInfo;
}

void VehicleInfoModel::NotifyDataChanged(std::string &info)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    if (mListenerVector.size() <= 0) {
        qDebug() << "No listener!";
        ResetData();
        return;
    }

    for (const auto &listener : mListenerVector) {
        listener->OnVehicleInfoUpdated(mVehicleInfo);
    }
}
