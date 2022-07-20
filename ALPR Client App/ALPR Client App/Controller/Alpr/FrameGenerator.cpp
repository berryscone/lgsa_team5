#include <climits>

#include <QDebug>
#include <QDir>
#include <QVector>
#include <QTextStream>
#include <QThread>

#include "FrameGenerator.h"


using namespace std;
using namespace cv;


#define MAX_ALPR_TIME_Q 100

FrameGenerator& FrameGenerator::GetInstance() {
    static FrameGenerator instance;
    return instance;
}

FrameGenerator::FrameGenerator() :
    mFrameCount(0),
    mElapsedDurationMs(0.0),
    mAvgTimePerFrameMs(0.0),
    mJitter(0.0),
    mFps(0)
{
    moveToThread(&mThread);
    mThread.start();
}

void FrameGenerator::Finalize() {
    mThread.quit();
    mThread.wait();
    mAlprAdapter.Finalize();
}

bool FrameGenerator::SetOpenFilePath(const QString filePath)
{
    const std::string filePathStdStr = filePath.toStdString();
    if (!mOpenCvAdapter.Create(filePathStdStr)) {
        return false;
    }
    mMsPerFrame = 1000 / mOpenCvAdapter.GetFrameRate();
    return true;
}

void FrameGenerator::Start()
{
    if (mQtimer) {
        mQtimer.release();
    }
    mQtimer = std::make_unique<QTimer>(this);
    connect(mQtimer.get(), &QTimer::timeout, this, &FrameGenerator::processFrameAndUpdateGUI);
    connect(&mThread, &QThread::finished, mQtimer.get(), &QTimer::stop);
    mQtimer->setTimerType(Qt::PreciseTimer);
    mQtimer->setInterval(mMsPerFrame);
    mQtimer->start();
}

void FrameGenerator::Pause()
{
    if (mQtimer) {
        mQtimer->stop();
    }
}

void FrameGenerator::Resume()
{
    if (mQtimer) {
        mQtimer->start();
    }
}

void FrameGenerator::Stop()
{
    if (mQtimer) {
        disconnect(mQtimer.get(), &QTimer::timeout, this, &FrameGenerator::processFrameAndUpdateGUI);
    }
    mOpenCvAdapter.Destroy();
}

void FrameGenerator::processFrameAndUpdateGUI()
{
    Mat frame;
    QVector<QRect> detectedRectLists;

    UpdateDebugInfo();

    mOpenCvAdapter.UpdateFrame(frame);

    //TODO : alpr worker�� ���� workerThread���� ALPR ó���� �ϵ��� �ؾ���
    TimePoint beforeAlprTime = mClock.now();
    mAlprAdapter.DetectAndShow(frame, detectedRectLists);
    TimePoint AfterAlprTime = mClock.now();
    auto count = std::chrono::duration_cast<std::chrono::milliseconds>(AfterAlprTime - beforeAlprTime).count();
    mALPRTimeQ.push((double)count);

    //Cal FPS, Avg.Time/Frame,Jitter for ALPR
    unsigned int q_cnt, i;
    double total_time, temp_time, sqr_time, mean_time;

    sqr_time = total_time = 0;
    q_cnt = mALPRTimeQ.size();

    if (q_cnt == MAX_ALPR_TIME_Q + 1)
    {
        mALPRTimeQ.pop();
        q_cnt = MAX_ALPR_TIME_Q;
    }
    for ( i = 0; i < q_cnt; i++)
    {
        temp_time = mALPRTimeQ.front();
        mALPRTimeQ.pop();
        total_time += temp_time;
        mALPRTimeQ.push(temp_time);
    }
    mean_time = total_time / q_cnt;
    mAvgTimePerFrameMs = mean_time;

    for (i = 0; i < q_cnt; i++)
    {
        temp_time = mALPRTimeQ.front();
        mALPRTimeQ.pop();
        sqr_time += pow((temp_time - mean_time), 2);
        mALPRTimeQ.push(temp_time);
    }
    mJitter = sqrt(sqr_time / q_cnt);

    QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    mVideoFrame = QPixmap::fromImage(qImage.copy(qImage.rect()));

    emit UpdateLaptopAppUi(mVideoFrame);

    const int current_frame = mOpenCvAdapter.GetCurrentFrame();
    const int total_frames = mOpenCvAdapter.GetTotalFrames();
    if (current_frame >= total_frames) {
        Stop();
        emit SignalVideoStopped();
    }
}

void FrameGenerator::UpdateDebugInfo()
{
    QString debugInfoStr;
    std::string convertStr;
    
    TimePoint currTime = mClock.now();
    auto count = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - mPrevTime).count();

    mPrevTime = currTime;
    mElapsedDurationMs += count;

    if (mElapsedDurationMs >= 1000) {
        mElapsedDurationMs = 0.0;
        mFps = mFrameCount;
        mFrameCount = 0;
    }
    else {
        mFrameCount++;
    }
    
    const int current_frame = mOpenCvAdapter.GetCurrentFrame() - 1;
    QTextStream(&debugInfoStr) << "FPS:" << QString::number(mFps, 'f', 0) << ",  Avg.Time/Frame(ms):" << QString::number(mAvgTimePerFrameMs, 'f', 2) << 
        ",  Jitter(ms):" << QString::number(mJitter, 'f', 2) << ",  Frame No.:" << current_frame;

    emit SignalUpdateDebugInfo(debugInfoStr);
}
