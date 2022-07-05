#ifndef OPENCVADAPTER_H
#define OPENCVADAPTER_H

#include <QObject>

#include "support/timing.h"
#include "motiondetector.h"
#include "alpr.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class OpenCvAdapter : public QObject
{
    Q_OBJECT

public:
    OpenCvAdapter();
    ~OpenCvAdapter() = default;

    void Create(std::string &filePath);
    void Destroy();
    void UpdateFrame(cv::Mat &frame);

    double GetFrameRate();

private:
    cv::VideoCapture mVideoFrame;
    std::string mFilePath;
    int mFrameRate;
    int mDelay;
};

#endif // OPENCVADAPTER_H
