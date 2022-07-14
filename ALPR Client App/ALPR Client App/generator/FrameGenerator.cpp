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
    mFilePath(""),
    mIsInitialized(false),
    mFrameCount(0),
    mTotalFrameCount(0),
    mElapsedDurationMs(0.0),
    mFps(0)
{

}

void FrameGenerator::SetOpenFilePath(std::string &filePath)
{
    mFilePath = filePath;
}

void FrameGenerator::Initialize()
{
    std::string country = "us";
    std::string currentPath = QDir::currentPath().toStdString();
    std::string configFile = currentPath + "/data/openalpr.conf";
    std::string runtimeDir = currentPath + "/data/runtime_data";

    if (mFilePath.empty()) {
        qDebug() << "filePath is empty()";
        return;
    }

    mOpenCvAdapter = std::make_unique<OpenCvAdapter>();
    mAlprAdapter = std::make_unique<AlprAdapter>();

    mOpenCvAdapter->Create(mFilePath);
    mAlprAdapter->Create(country, configFile, runtimeDir);
}

void FrameGenerator::Finalize()
{
    mQtimer->stop();
    mOpenCvAdapter->Destroy();
    mAlprAdapter->Destroy();
}

void FrameGenerator::Start()
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    if (mIsInitialized) {
        qDebug() << "FrameGeneartor is already initialized";
        return;
    }

    Initialize();

    mQtimer = std::make_unique<QTimer>(this);
    connect(mQtimer.get(), SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    mQtimer->setInterval(1000/mOpenCvAdapter->GetFrameRate());
    mQtimer->setTimerType(Qt::PreciseTimer);
    mQtimer->start();

    mIsInitialized = true;
}

void FrameGenerator::Resume()
{ 
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    mQtimer->start();
}

void FrameGenerator::Pause()
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    mQtimer->stop();
}

void FrameGenerator::Stop()
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    Finalize();
    mIsInitialized = false;
}

void FrameGenerator::processFrameAndUpdateGUI()
{
    Mat frame;
    QVector<QRect> detectedRectLists;

    UpdateDebugInfo();

    mOpenCvAdapter->UpdateFrame(frame);

    //TODO : alpr worker를 만들어서 workerThread에서 ALPR 처리를 하도록 해야함
    mAlprAdapter->DetectAndShow(frame, detectedRectLists);

    QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    mVideoFrame = QPixmap::fromImage(qImage.copy(qImage.rect()));

    emit UpdateLaptopAppUi(mVideoFrame);

    //NOTE : frame data의 경우 ui thread로 바로 전달하는게 나을거 같음
    //FrameModel::GetInstance().SetFrameData(qImage);
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
