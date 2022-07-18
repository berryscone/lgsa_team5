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


#define MAX_ALPR_TIME_Q 100

FrameGenerator::FrameGenerator() :
    mFilePath(""),
    mIsInitialized(false),
    mFrameCount(0),
    mTotalFrameCount(0),
    mElapsedDurationMs(0.0),
    mAvgTimePerFrameMs(0.0),
    mJitter(0.0),
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
    TimePoint beforeAlprTime = mClock.now();
    mAlprAdapter->DetectAndShow(frame, detectedRectLists);
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

    mTotalFrameCount++;

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
    //mAvgTimePerFrameMs = mElapsedDurationMs / mFrameCount;

    if (mElapsedDurationMs > 1000) {
        mElapsedDurationMs = 0.0;
        mFps = mFrameCount;
        mFrameCount = 0;
    }
    

    if (mTotalFrameCount >= UINT_MAX) {
        qDebug() << "mTotalFrameCount overflow";
        mTotalFrameCount = 0;
    }

    QTextStream(&debugInfoStr) << "FPS:" << QString::number(mFps, 'f', 0) << ",  Avg.Time/Frame(ms):" << QString::number(mAvgTimePerFrameMs, 'f', 2) << ",  Jitter(ms):" << QString::number(mJitter, 'f', 2) << ",  Frame No.:" << mTotalFrameCount;
    convertStr = debugInfoStr.toStdString();

    DebugInfoModel::GetInstance().SetDebugInfoData(convertStr);
}
