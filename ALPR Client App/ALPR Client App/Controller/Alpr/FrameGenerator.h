#pragma once
#include <queue>

#include <QTimer>
#include <QImage>
#include <QPixmap>

#include "Controller/Alpr/OpenCvAdapter.h"
#include "Controller/Alpr/AlprAdapter.h"


using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::steady_clock::time_point;

class FrameGenerator : public QObject
{
    Q_OBJECT

public:
    static FrameGenerator& GetInstance();
    void Finalize();

    bool SetOpenFilePath(const QString filePath);

signals :
    void UpdateLaptopAppUi(QPixmap pixmap);
    void SignalUpdateDebugInfo(QString info);
    void SignalVideoStopped();

public slots:
    void Start();
    void Pause();
    void Resume();
    void Stop();
    void processFrameAndUpdateGUI();

private :
    FrameGenerator();
    void UpdateDebugInfo();

    QThread mThread;
    std::unique_ptr<QTimer> mQtimer;
    OpenCvAdapter mOpenCvAdapter;
    AlprAdapter mAlprAdapter;
    QPixmap mVideoFrame;

    //to check FPS
    Clock mClock;
    TimePoint mPrevTime;
    unsigned int mFrameCount;
    double mElapsedDurationMs;
    int mMsPerFrame;
    double mAvgTimePerFrameMs;
    double mJitter;
    double mFps;
    std::queue<double> mALPRTimeQ;
};
