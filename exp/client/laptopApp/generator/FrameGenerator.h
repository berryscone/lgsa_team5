#ifndef FRAMEGENERATOR_H
#define FRAMEGENERATOR_H

#include "OpenCvAdapter.h"
#include "alpr/AlprAdapter.h"

#include <QTimer>
#include <QImage>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::steady_clock::time_point;

class FrameGenerator : public QObject
{
    Q_OBJECT

public:
    FrameGenerator();
    ~FrameGenerator() = default;

    void Start(std::string &filePath);
    void Resume();
    void Pause();
    void Stop();

public slots:
    void processFrameAndUpdateGUI();

private :
    QTimer* mQtimer;
    std::unique_ptr<OpenCvAdapter> mOpenCvAdapter;
    std::unique_ptr<AlprAdapter> mAlprAdapter;

    //to check FPS
    Clock mClock;
    TimePoint mPrevTime;
    unsigned int mFrameCount;
    unsigned int mTotalFrameCount;
    double mElapsedDurationMs;
    unsigned int mFps;

    void UpdateDebugInfo();
};

#endif // FRAMEGENERATOR_H
