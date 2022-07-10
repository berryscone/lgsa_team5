#pragma once

#include <mutex>
#include <vector>

#include <QVector>
#include <QRect>

class IVehicleInfoListener {
public:
    virtual ~IVehicleInfoListener() = default;
    virtual void OnVehicleInfoUpdated(const std::string &info) = 0;
};

class VehicleInfoModel
{
public:
    static VehicleInfoModel& GetInstance();
    void ResetData();
    void ResetListener();

    void SubscribeListener(IVehicleInfoListener *pListener);
    void UnSubscribeListener(IVehicleInfoListener *pListener);

    void SetVehicleInfoData(std::string &info);
    std::string GetVehicleInfoData();

private:
    VehicleInfoModel();

    void NotifyDataChanged(std::string &info);

    std::mutex mMutex;
    std::mutex mListenerVectorMutex;

    std::vector<IVehicleInfoListener*> mListenerVector;

    std::string mVehicleInfo;
};
