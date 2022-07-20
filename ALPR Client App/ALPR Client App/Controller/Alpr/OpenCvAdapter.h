#pragma once

#include <QObject>

#include <openalpr/support/timing.h>
#include <openalpr/motiondetector.h>
#include <openalpr/alpr.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class OpenCvAdapter : public QObject
{
    Q_OBJECT

public:
    OpenCvAdapter() = default;
    ~OpenCvAdapter() = default;

    bool Create(const std::string &filePath);
    void Destroy();
    void UpdateFrame(cv::Mat &frame);

    double GetFrameRate();
    inline int GetTotalFrames() { return mTotalFrames; }
    int GetCurrentFrame();

private:
    cv::VideoCapture mVideoFrame;
    int mFrameRate;
    int mTotalFrames;
};
