#ifndef FRAMEMODEL_H
#define FRAMEMODEL_H

#include <mutex>
#include <vector>

#include <QVector>
#include <QRect>
#include <QImage>

#include <opencv2/opencv.hpp>

class IFrameListener {
public:
    virtual ~IFrameListener() = default;
    virtual void OnFrameUpdated(const QImage &frame) = 0;
};

class FrameModel
{
public:
    static FrameModel& GetInstance();
    void ResetData();
    void ResetListener();

    void SubscribeListener(IFrameListener *pListener);
    void UnSubscribeListener(IFrameListener *pListener);

    void SetFrameData(const QImage &frame);
    QImage GetFrameData();

private:
    FrameModel();

    void NotifyDataChanged(const QImage &frame);

    std::mutex mMutex;
    std::mutex mListenerVectorMutex;

    std::vector<IFrameListener*> mListenerVector;

    QImage mFrame;
};

#endif // FRAMEMODEL_H
