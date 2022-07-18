#include "AlprAdapter.h"

#include <QDebug>

using namespace alpr;
using namespace std;
using namespace cv;

AlprAdapter::AlprAdapter() :
    mUseMotiondetection(0),
    mVehicleQueryProvider(NetworkManager::GetInstance())
{
    std::string country = "us";
    std::string currentPath = QDir::currentPath().toStdString();
    std::string configFile = currentPath + "/data/openalpr.conf";
    std::string runtimeDir = currentPath + "/data/runtime_data";

    mAlpr = std::make_unique<alpr::Alpr>(country, configFile, runtimeDir);
    mAlpr->setTopN(2);
    if (!mAlpr->isLoaded()) {
        // TODO: Handle Exception
        qCritical() << "Error loading OpenALPR";
        return;
    }

    connect(this, &AlprAdapter::SignalRequestVehicleQuery, 
        &mVehicleQueryProvider, &IVehicleQueryProvider::RequestVehicleQuery);
}

AlprAdapter::~AlprAdapter()
{
    // mAlpr.release();
}

void AlprAdapter::DetectAndShow(cv::Mat &frame, QVector<QRect> &detectedRectLists)
{
    alpr::AlprResults alprResults;

    if (mFrameno == 0) {
        mMotiondetector.ResetMotionDetection(&frame);
        mFrameno++;
    }

    DetectAndShowCore(mAlpr, frame, "", false, detectedRectLists, alprResults);

    cv::putText(frame, mText,
        cv::Point(10, frame.rows - 10), //top-left position
        FONT_HERSHEY_COMPLEX_SMALL, 0.5,
        Scalar(0, 255, 0), 0, LINE_AA, false);

    if (detectedRectLists.size() > 0) {
        Rect cropRect{detectedRectLists.at(0).x(), detectedRectLists.at(0).y(), detectedRectLists.at(0).width(), detectedRectLists.at(0).height()};

        AdjustCropRect(frame, cropRect);
        cv::Mat licensePlateImage = frame(cropRect);
        QString licensePlateStr = alprResults.plates[0].bestPlate.characters.c_str();

        for (int i = 0; i < alprResults.plates.size(); ++i) {
            //qDebug() << alprResults.plates[i].bestPlate.characters.c_str();
            emit SignalRequestVehicleQuery(licensePlateImage,
                alprResults.plates[i].bestPlate.characters.c_str());
        }
    }
}

void AlprAdapter::AdjustCropRect(cv::Mat &frame, Rect &rect)
{
    if (rect.x < 0) rect.x = 0;
    if (rect.y < 0) rect.y = 0;
    if (rect.width > frame.cols) rect.width = frame.cols;
    if (rect.height > frame.rows) rect.height = frame.rows;
    if (rect.x + rect.width > frame.cols) rect.width = frame.cols - rect.x;
    if (rect.y + rect.height > frame.rows) rect.height = frame.rows - rect.y;
}

bool AlprAdapter::DetectAndShowCore(std::unique_ptr<alpr::Alpr> & alpr, cv::Mat frame,
                                    std::string region, bool writeJson, QVector<QRect> &detectedRectLists, 
                                    alpr::AlprResults& results)
{
    timespec startTime;
    timespec endTime;
    getTimeMonotonic(&startTime);
    unsigned short SendPlateStringLength;
    ssize_t result;
    std::vector<AlprRegionOfInterest> regionsOfInterest;

    if (mUseMotiondetection) {
        cv::Rect rectan = mMotiondetector.MotionDetect(&frame);
        if (rectan.width > 0) {
            regionsOfInterest.push_back(AlprRegionOfInterest(rectan.x, rectan.y, rectan.width, rectan.height));
        }
    } else {
        regionsOfInterest.push_back(AlprRegionOfInterest(0, 0, frame.cols, frame.rows));
    }

    if (regionsOfInterest.size() > 0) {
        results = alpr->recognize(frame.data, (int)frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
    }

    getTimeMonotonic(&endTime);
    double totalProcessingTime = diffclock(startTime, endTime);

    for (int i = 0; i < results.plates.size(); i++) {
        char textbuffer[1024];
        std::vector<cv::Point2f> pointset;
        for (int z = 0; z < 4; z++)
            pointset.push_back(Point2i(results.plates[i].plate_points[z].x, results.plates[i].plate_points[z].y));
        cv::Rect rect = cv::boundingRect(pointset);

        QRect qRect(rect.x, rect.y, rect.width, rect.height);
        detectedRectLists.push_back(qRect);

        cv::rectangle(frame, rect, cv::Scalar(0, 255, 0),2);
        sprintf_s(textbuffer, "%s - %.2f", results.plates[i].bestPlate.characters.c_str(), results.plates[i].bestPlate.overall_confidence);

        cv::putText(frame, textbuffer,
            cv::Point(rect.x, rect.y-5), //top-left position
            FONT_HERSHEY_COMPLEX_SMALL, 1,
            Scalar(0, 255, 0), 0, LINE_AA, false);
    }

    return results.plates.size() > 0;
}
