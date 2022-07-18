#pragma once

#include <QObject>
#include <QRect>

#include <openalpr/support/timing.h>
#include <openalpr/motiondetector.h>
#include <openalpr/alpr.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <queue>

#include "network/NetworkInterfaces.h"
#include "network/NetworkManager.h"

class AlprAdapter : public QObject
{
    Q_OBJECT

signals:
    void SignalRequestVehicleQuery(const cv::Mat plate_image, const QString plate_number, const int retry_cnt = 0);

public:
    AlprAdapter();
    ~AlprAdapter() = default;

    void Create(std::string &country, std::string &configFile, std::string &runtimeDir);
    void Destroy();
    void DetectAndShow(cv::Mat &frame, QVector<QRect> &detectedRectLists);

private:
    void AdjustCropRect(cv::Mat &frame, cv::Rect &rect);
    bool DetectAndShowCore(std::unique_ptr<alpr::Alpr> & alpr, cv::Mat frame,
                           std::string region, bool writeJson, QVector<QRect> &detectedRectLists,
                           alpr::AlprResults &results);

    std::unique_ptr<alpr::Alpr> mAlpr;
    alpr::MotionDetector mMotiondetector;
    bool mUseMotiondetection;
    int mFrameno;
    char mText[1024] = "";

    queue<string> mPlateStringQ;

    IVehicleQueryProvider& mVehicleQueryProvider;
};
