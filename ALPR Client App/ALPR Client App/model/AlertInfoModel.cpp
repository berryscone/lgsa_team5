#include "AlertInfoModel.h"

#include <QDebug>

AlertInfoModel &AlertInfoModel::GetInstance()
{
    static AlertInfoModel instance;
    return instance;
}

AlertInfoModel::AlertInfoModel()
{
    ResetData();
    ResetListener();
}

void AlertInfoModel::ResetData()
{

}

void AlertInfoModel::ResetListener()
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.clear();
    ResetData();
}

void AlertInfoModel::SubscribeListener(IAlertInfoListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.push_back(pListener);
}

void AlertInfoModel::UnSubscribeListener(IAlertInfoListener *pListener)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    mListenerVector.erase(
                std::remove(mListenerVector.begin(),
                            mListenerVector.end(), pListener),
                            mListenerVector.end());
}

void AlertInfoModel::SetAlertInfoData(QImage &licensePlateImage, std::string &alertInfo)
{
    NotifyDataChanged(licensePlateImage, alertInfo);
}

void AlertInfoModel::NotifyDataChanged(QImage &licensePlateImage, std::string &alertInfo)
{
    std::unique_lock<std::mutex> lock(mListenerVectorMutex);
    if (mListenerVector.size() <= 0) {
        qDebug() << "No listener!";
        ResetData();
        return;
    }

    for (const auto &listener : mListenerVector) {
        listener->OnAlertInfoUpdated(licensePlateImage, alertInfo);
    }
}
