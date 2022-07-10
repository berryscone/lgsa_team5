#include "FrameModel.h"

#include <QDebug>

FrameModel &FrameModel::GetInstance()
{
    static FrameModel instance;
    return instance;
}

FrameModel::FrameModel()
{
    ResetData();
    ResetListener();
}

void FrameModel::ResetData()
{

}

void FrameModel::ResetListener()
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.clear();
    ResetData();
}

void FrameModel::SubscribeListener(IFrameListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.push_back(pListener);
}

void FrameModel::UnSubscribeListener(IFrameListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.erase(
                std::remove(mListenerVector.begin(),
                            mListenerVector.end(), pListener),
                            mListenerVector.end());
}

void FrameModel::SetFrameData(const QImage &frame)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mFrame = frame;
    mMutex.unlock();

    NotifyDataChanged(frame);
}

QImage FrameModel::GetFrameData()
{
    std::unique_lock<std::mutex> lock(mMutex);
    return mFrame;
}

void FrameModel::NotifyDataChanged(const QImage &frame)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    if (mListenerVector.size() <= 0) {
        qDebug() << "No listener!";
        ResetData();
        return;
    }

    for (const auto &listener : mListenerVector) {
        listener->OnFrameUpdated(frame);
    }
}
