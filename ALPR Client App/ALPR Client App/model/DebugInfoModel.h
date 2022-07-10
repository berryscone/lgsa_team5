#pragma once

#include <mutex>
#include <vector>

#include <QVector>
#include <QRect>

class IDebugInfoListener {
public:
    virtual ~IDebugInfoListener() = default;
    virtual void OnDebugInfoUpdated(const std::string &info) = 0;
};

class DebugInfoModel
{
public:
    static DebugInfoModel& GetInstance();
    void ResetData();
    void ResetListener();

    void SubscribeListener(IDebugInfoListener *pListener);
    void UnSubscribeListener(IDebugInfoListener *pListener);

    void SetDebugInfoData(std::string &info);
    std::string GetDebugInfoData();

private:
    DebugInfoModel();

    void NotifyDataChanged(std::string &info);

    std::mutex mMutex;
    std::mutex mListenerVectorMutex;

    std::vector<IDebugInfoListener*> mListenerVector;

    std::string mDebugInfo;
};
