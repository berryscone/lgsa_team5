#pragma once

#include <mutex>
#include <vector>

#include <QVector>
#include <QRect>

#include <opencv2/opencv.hpp>

class IRecentPlatesListener {
public:
    virtual ~IRecentPlatesListener() = default;
    virtual void OnRecentPlatesUpdated(const cv::Mat &frame, const QVector<QRect> &rectLists) = 0;
};

class RecentPlatesModel
{
public:
    static RecentPlatesModel& GetInstance();
    void ResetData();
    void ResetListener();

    void SubscribeListener(IRecentPlatesListener *pListener);
    void UnSubscribeListener(IRecentPlatesListener *pListener);

    void SetRecentPlatesData(const cv::Mat &frame, const QVector<QRect> &rectLists);
    QVector<QRect> GetRecentPlatesData();

private:
    RecentPlatesModel();

    void NotifyDataChanged(const cv::Mat &frame, const QVector<QRect> &rectLists);

    std::mutex mMutex;
    std::mutex mListenerVectorMutex;

    std::vector<IRecentPlatesListener*> mListenerVector;

    cv::Mat mFrame;
    QVector<QRect> mRecentPlates;
};
