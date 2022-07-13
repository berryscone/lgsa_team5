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

void RecentPlatesModel::SetRecentPlatesData(const cv::Mat &frame, const QVector<QRect> &rectLists)
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mFrame = frame;
        mRecentPlates = rectLists;
    }
    NotifyDataChanged(frame, rectLists);
}

QVector<QRect> RecentPlatesModel::GetRecentPlatesData()
{
    std::unique_lock<std::mutex> lock(mMutex);
    return mRecentPlates;
}

void RecentPlatesModel::NotifyDataChanged(const cv::Mat &frame, const QVector<QRect> &rectLists)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    if (mListenerVector.size() <= 0) {
        qDebug() << "No listener!";
        ResetData();
        return;
    }

    for (const auto &listener : mListenerVector) {
        listener->OnRecentPlatesUpdated(frame, mRecentPlates);
    }
}
