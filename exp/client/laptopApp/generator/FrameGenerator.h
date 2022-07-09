#ifndef FRAMEGENERATOR_H
#define FRAMEGENERATOR_H

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

    void SetOpenFilePath(std::string &filePath);

signals :
    void UpdateLaptopAppUi(QPixmap pixmap);

public slots:
    void Start();
    void Resume();
    void Pause();
    void Stop();
    void processFrameAndUpdateGUI();

private :
    void Initialize();
    void Finalize();

    std::unique_ptr<QTimer> mQtimer;
    std::unique_ptr<OpenCvAdapter> mOpenCvAdapter;
    std::unique_ptr<AlprAdapter> mAlprAdapter;

    std::string mFilePath;
    QPixmap mVideoFrame;
    bool mIsInitialized;

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
