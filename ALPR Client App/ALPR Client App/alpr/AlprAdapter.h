#pragma once

#include <QObject>
#include <QRect>

#include <openalpr/support/timing.h>
#include <openalpr/motiondetector.h>
#include <openalpr/alpr.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class AlprAdapter : public QObject
{
    Q_OBJECT

public:
    AlprAdapter();
    ~AlprAdapter() = default;

    void Create(std::string &country, std::string &configFile, std::string &runtimeDir);
    void Destroy();
    void DetectAndShow(cv::Mat &frame, QVector<QRect> &detectedRectLists);

private:
    bool DetectAndShowCore(std::unique_ptr<alpr::Alpr> & alpr, cv::Mat frame,
                           std::string region, bool writeJson, QVector<QRect> &detectedRectLists);

    std::unique_ptr<alpr::Alpr> mAlpr;
    alpr::MotionDetector mMotiondetector;
    bool mUseMotiondetection;
    int mFrameno;
    char mText[1024] = "";
};
