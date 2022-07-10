#include "OpenCvAdapter.h"

#include <QDebug>

using namespace alpr;
using namespace std;
using namespace cv;

OpenCvAdapter::OpenCvAdapter()
{

}

void OpenCvAdapter::Create(std::string &filePath)
{
    if (mVideoFrame.isOpened()) {
        mVideoFrame.release();
        return;
    }

    if (filePath.empty()) {
        return;
    }

    if (!mVideoFrame.open(filePath)) {
        qDebug() << "Video Error! Make sure you entered a correct and supported mVideoFrame file path";
        return;
    }

    if (!mVideoFrame.isOpened()) {
        qDebug() << "Video Error! Make sure you entered a correct and supported mVideoFrame file path";
        return;
    } else {
        mFrameRate = (int) mVideoFrame.get(CAP_PROP_FPS);
        mDelay = (1000/mFrameRate);
    }
}

void OpenCvAdapter::Destroy()
{
    if (mVideoFrame.isOpened()) {
        qDebug() << "OpenCvAdapter::Destroy";
        mVideoFrame.release();
    }
}

void OpenCvAdapter::UpdateFrame(cv::Mat &frame)
{
    if (mVideoFrame.isOpened()) {
        mVideoFrame >> frame;
    }
}

double OpenCvAdapter::GetFrameRate()
{
    if (mVideoFrame.isOpened()) {
        return mVideoFrame.get(CAP_PROP_FPS);
    } else {
        return 0.0;
    }
}
