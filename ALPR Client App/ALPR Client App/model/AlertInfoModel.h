#pragma once

#include <mutex>
#include <vector>

#include <QVector>
#include <QImage>

class IAlertInfoListener {
public:
    virtual ~IAlertInfoListener() = default;
    virtual void OnAlertInfoUpdated(const QImage &licensePlateImage, const std::string &alertInfo) = 0;
};

class AlertInfoModel
{
public:
    static AlertInfoModel& GetInstance();
    void ResetData();
    void ResetListener();

    void SubscribeListener(IAlertInfoListener *pListener);
    void UnSubscribeListener(IAlertInfoListener *pListener);

    void SetAlertInfoData(QImage &licensePlateImage, std::string &alertInfo);

private:
    AlertInfoModel();

    void NotifyDataChanged(QImage &licensePlateImage, std::string &alertInfo);

    std::mutex mMutex;
    std::mutex mListenerVectorMutex;

    std::vector<IAlertInfoListener*> mListenerVector;

    std::string mAlertInfo;
};
