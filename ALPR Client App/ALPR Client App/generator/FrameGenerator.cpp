#include "FrameGenerator.h"
#include "model/DebugInfoModel.h"

#include <climits>

#include <QDebug>
#include <QDir>
#include <QVector>
#include <QTextStream>
#include <QThread>

using namespace std;
using namespace cv;

FrameGenerator::FrameGenerator() :
    mFrameCount(0),
    mElapsedDurationMs(0.0),
    mFps(0)
{
}

void FrameGenerator::SetOpenFilePath(const QString filePath)
{
    const std::string filePathStdStr = filePath.toStdString();
    mOpenCvAdapter.Create(filePathStdStr);
    mMsPerFrame = 1000 / mOpenCvAdapter.GetFrameRate();
}

void FrameGenerator::Start()
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    mQtimer = std::make_unique<QTimer>(this);
    connect(mQtimer.get(), &QTimer::timeout, this, &FrameGenerator::processFrameAndUpdateGUI);
    mQtimer->setInterval(mMsPerFrame);
    mQtimer->setTimerType(Qt::PreciseTimer);
    mQtimer->start();
}

void FrameGenerator::Pause()
{
    qDebug() << "Function Name: " << Q_FUNC_INFO << ", tid:" << QThread::currentThreadId();
    mQtimer->stop();
}

void FrameGenerator::Resume()
{ 
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    mQtimer->start();
}

void FrameGenerator::Stop()
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    mQtimer->stop();
    mOpenCvAdapter.Destroy();
}

void FrameGenerator::processFrameAndUpdateGUI()
{
    Mat frame;
    QVector<QRect> detectedRectLists;

    UpdateDebugInfo();

    mOpenCvAdapter.UpdateFrame(frame);

    mAlprAdapter.DetectAndShow(frame, detectedRectLists);

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
    mFrameCount++;
    mElapsedDurationMs += count;

    if (mElapsedDurationMs > 1000) {
        mElapsedDurationMs = 0.0;
        mFps = mFrameCount;
        mFrameCount = 0;
    }

    const int current_frame = mOpenCvAdapter.GetCurrentFrame() - 1;
    QTextStream(&debugInfoStr) << "FPS:" << mFps << ", Diff Time:" << count << "(ms)" << ", FrameNo:" << current_frame << ", Diff Avg:TBD, Jitter:TBD";
    convertStr = debugInfoStr.toStdString();

    DebugInfoModel::GetInstance().SetDebugInfoData(convertStr);
}
