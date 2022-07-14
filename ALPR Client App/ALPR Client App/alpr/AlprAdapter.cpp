#include "AlprAdapter.h"
#include "model/RecentPlatesModel.h"
//#include "model/VehicleInfoModel.h" //TODO : 임시 테스트용. 추후 NetworkManager에서 사용해야함

#include <QDebug>

using namespace alpr;
using namespace std;
using namespace cv;

AlprAdapter::AlprAdapter() :
    mUseMotiondetection(0),
    mVehicleQueryProvider(NetworkManager::GetInstance())
{
    connect(this, &AlprAdapter::SignalRequestVehicleQuery, 
        &mVehicleQueryProvider, &IVehicleQueryProvider::RequestVehicleQuery);
}

void AlprAdapter::Create(std::string &country, std::string &configFile, std::string &runtimeDir)
{
    mAlpr = std::make_unique<alpr::Alpr>(country, configFile, runtimeDir);

    mAlpr->setTopN(2);

    if (mAlpr->isLoaded() == false) {
        std::cerr << "Error loading OpenALPR" << std::endl;
        return;
    }
}

void AlprAdapter::Destroy()
{
    qDebug() << "AlprAdapter::Destroy";
    mAlpr.release();
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
        cv::Mat licensePlateImage = frame(cropRect);
        QString licensePlateStr = alprResults.plates[0].bestPlate.characters.c_str();

        QImage qImage(licensePlateImage.data, licensePlateImage.cols, licensePlateImage.rows, licensePlateImage.step, QImage::Format_BGR888);
        RecentPlatesModel::GetInstance().SetRecentPlatesData(qImage, licensePlateStr);

        // TODO: crop 된 이미지와 plate number 전달
        for (int i = 0; i < alprResults.plates.size(); ++i) {
            //qDebug() << alprResults.plates[i].bestPlate.characters.c_str();
            emit SignalRequestVehicleQuery(licensePlateImage,
                alprResults.plates[i].bestPlate.characters.c_str());
        }
    }
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

    //std::cout << "Total Time to process image: " << totalProcessingTime << "ms." << std::endl;

    if (writeJson) {
        std::cout << alpr->toJson(results) << std::endl;
    } else {
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
#if 0
            std::cout << "plate" << i << ": " << results.plates[i].topNPlates.size() << " results";
            if (measureProcessingTime)
                std::cout << " -- Processing Time = " << results.plates[i].processing_time_ms << "ms.";
            std::cout << std::endl;

            if (results.plates[i].regionConfidence > 0)
                std::cout << "State ID: " << results.plates[i].region << " (" << results.plates[i].regionConfidence << "% confidence)" << std::endl;

            for (int k = 0; k < results.plates[i].topNPlates.size(); k++)
            {
                // Replace the multiline newline character with a dash
                std::string no_newline = results.plates[i].topNPlates[k].characters;
                std::replace(no_newline.begin(), no_newline.end(), '\n', '-');

                std::cout << "    - " << no_newline << "\t confidence: " << results.plates[i].topNPlates[k].overall_confidence;
                if (templatePattern.size() > 0 || results.plates[i].regionConfidence > 0)
                    std::cout << "\t pattern_match: " << results.plates[i].topNPlates[k].matches_template;

                std::cout << std::endl;
            }
#endif
        }
    }
    return results.plates.size() > 0;
}
