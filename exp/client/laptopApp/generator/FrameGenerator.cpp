#include "FrameGenerator.h"
#include "model/DebugInfoModel.h"
#include "model/FrameModel.h"

#include <climits>

#include <QDebug>
#include <QDir>
#include <QVector>
#include <QTextStream>

using namespace std;
using namespace cv;

FrameGenerator::FrameGenerator() :
    mFrameCount(0),
    mTotalFrameCount(0),
    mElapsedDurationMs(0.0),
    mFps(0)
{
    mQtimer = new QTimer(this);
    mOpenCvAdapter = std::make_unique<OpenCvAdapter>();
    mAlprAdapter = std::make_unique<AlprAdapter>();
}

void FrameGenerator::Start(std::string &filePath)
{
    std::string country = "us";
    std::string buildType = "";

#ifdef QT_DEBUG
    buildType = "/debug";
#else
    buildType = "/release";
#endif

    std::string currentPath = QDir::currentPath().toStdString();
    std::string configFile = currentPath + buildType + "/openalpr.conf";
    std::string runtimeDir = currentPath + buildType + "/runtime_data";

    mOpenCvAdapter->Create(filePath);
    mAlprAdapter->Create(country, configFile, runtimeDir);

    connect(mQtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    mQtimer->setInterval(1000/mOpenCvAdapter->GetFrameRate());
    mQtimer->setTimerType(Qt::PreciseTimer);
    mQtimer->start();
}

void FrameGenerator::Resume()
{
    mQtimer->start();
}

void FrameGenerator::Pause()
{
    mQtimer->stop();
}

void FrameGenerator::Stop()
{
    mQtimer->stop();
    mOpenCvAdapter->Destroy();
}

void FrameGenerator::processFrameAndUpdateGUI()
{
    Mat frame;
    QVector<QRect> detectedRectLists;

    UpdateDebugInfo();

    mOpenCvAdapter->UpdateFrame(frame);
    mAlprAdapter->DetectAndShow(frame, detectedRectLists);

    QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);

    FrameModel::GetInstance().SetFrameData(qImage);
}

void FrameGenerator::UpdateDebugInfo()
{
    QString debugInfoStr;
    std::string convertStr;
    TimePoint currTime = mClock.now();
    auto count = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - mPrevTime).count();

    mPrevTime = currTime;
    mFrameCount++;
    mTotalFrameCount++;
    mElapsedDurationMs += count;

    if (mElapsedDurationMs > 1000) {
        mElapsedDurationMs = 0.0;
        mFps = mFrameCount;
        mFrameCount = 0;
    }

    if (mTotalFrameCount >= UINT_MAX) {
        qDebug() << "mTotalFrameCount overflow";
        mTotalFrameCount = 0;
    }

    QTextStream(&debugInfoStr) << "FPS:" << mFps << ", Diff Time:" << count << "(ms)" << ", FrameNo:" << mTotalFrameCount << ", Diff Avg:TBD, Jitter:TBD";
    convertStr = debugInfoStr.toStdString();

    DebugInfoModel::GetInstance().SetDebugInfoData(convertStr);
}
