#include "OpenCvAdapter.h"

#include <QDebug>

using namespace alpr;
using namespace std;
using namespace cv;

bool OpenCvAdapter::Create(const std::string &filePath)
{
    if (mVideoFrame.isOpened()) {
        mVideoFrame.release();
        return false;
    }

    if (filePath.empty()) {
        return false;
    }

    if (!mVideoFrame.open(filePath)) {
        qDebug() << "Video Error! Make sure you entered a correct and supported mVideoFrame file path";
        return false;
    }

    if (!mVideoFrame.isOpened()) {
        qDebug() << "Video Error! Make sure you entered a correct and supported mVideoFrame file path";
        return false;
    }

    mFrameRate = static_cast<int>(mVideoFrame.get(CAP_PROP_FPS));
    mTotalFrames = static_cast<int>(mVideoFrame.get(CAP_PROP_FRAME_COUNT));
    return true;
}

void OpenCvAdapter::Destroy()
{
    if (mVideoFrame.isOpened()) {
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

int OpenCvAdapter::GetCurrentFrame() {
    return static_cast<int>(mVideoFrame.get(CAP_PROP_POS_FRAMES));
}
