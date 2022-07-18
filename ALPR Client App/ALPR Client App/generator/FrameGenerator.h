#pragma once

#include "OpenCvAdapter.h"
#include "alpr/AlprAdapter.h"

#include <QTimer>
#include <QImage>
#include <QPixmap>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::steady_clock::time_point;

class FrameGenerator : public QObject
{
    Q_OBJECT

public:
    FrameGenerator();
    ~FrameGenerator() = default;

    void SetOpenFilePath(const QString filePath);

signals :
    void UpdateLaptopAppUi(QPixmap pixmap);
    void SignalVideoStopped();

public slots:
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void processFrameAndUpdateGUI();

private :
    std::unique_ptr<QTimer> mQtimer;
    OpenCvAdapter mOpenCvAdapter;
    AlprAdapter mAlprAdapter;

    QPixmap mVideoFrame;

    //to check FPS
    Clock mClock;
    TimePoint mPrevTime;
    unsigned int mFrameCount;
    double mElapsedDurationMs;
    unsigned int mFps;
    int mMsPerFrame;

    void UpdateDebugInfo();
};
