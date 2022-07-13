#pragma once

#include <QObject>
#include <QRect>

#include "network/NetworkManager.h"
#include <openalpr/support/timing.h>
#include <openalpr/motiondetector.h>
#include <openalpr/alpr.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class AlprAdapter : public QObject
{
    Q_OBJECT

signals:
    void signalRequestQuery(QString url, QString licensePlate);

public:
    AlprAdapter();
    ~AlprAdapter() = default;

    void Create(std::string &country, std::string &configFile, std::string &runtimeDir);
    void Destroy();
    void DetectAndShow(cv::Mat &frame, QVector<QRect> &detectedRectLists);

private:
    //TODO : networkManager instance 생성을 main.cpp에서 하도록 추후 수정 예정 --- START
    void CreateNetworkManager();
    void DestroyNetworkManager();
    void AsyncRequestQuery(QString url, QString licensePlate);
    //END

    AlprResults DetectAndShowCore(std::unique_ptr<alpr::Alpr> & alpr, cv::Mat frame,
                           std::string region, bool writeJson, QVector<QRect> &detectedRectLists);

    std::unique_ptr<alpr::Alpr> mAlpr;
    alpr::MotionDetector mMotiondetector;
    bool mUseMotiondetection;
    int mFrameno;
    char mText[1024] = "";

    std::unique_ptr<QThread> mNetworkManagerThread;
    std::unique_ptr<NetworkManager> mNetworkManager;
};
