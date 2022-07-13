#include "DebugInfoModel.h"

#include <QDebug>


DebugInfoModel &DebugInfoModel::GetInstance()
{
    static DebugInfoModel instance;
    return instance;
}

DebugInfoModel::DebugInfoModel()
{
    ResetData();
    ResetListener();
}

void DebugInfoModel::ResetData()
{

}

void DebugInfoModel::ResetListener()
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.clear();
    ResetData();
}

void DebugInfoModel::SubscribeListener(IDebugInfoListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.push_back(pListener);
}

void DebugInfoModel::UnSubscribeListener(IDebugInfoListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.erase(
                std::remove(mListenerVector.begin(),
                            mListenerVector.end(), pListener),
                            mListenerVector.end());
}

void DebugInfoModel::SetDebugInfoData(std::string &info)
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mDebugInfo = info;
    }
    NotifyDataChanged(info);
}

std::string DebugInfoModel::GetDebugInfoData()
{
    std::unique_lock<std::mutex> lock(mMutex);
    return mDebugInfo;
}

void DebugInfoModel::NotifyDataChanged(std::string &info)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    if (mListenerVector.size() <= 0) {
        qDebug() << "No listener!";
        ResetData();
        return;
    }

    for (const auto &listener : mListenerVector) {
        listener->OnDebugInfoUpdated(mDebugInfo);
    }
}
