#include "RecentPlatesMsgHandler.h"

#include <QDebug>

RecentPlatesMsgHandler::RecentPlatesMsgHandler()
{

}

RecentPlatesMsgHandler::~RecentPlatesMsgHandler()
{

}

void RecentPlatesMsgHandler::OnStartHandler()
{
    qDebug() << "RecentPlatesMsgHandler::OnStartHandler";
    RecentPlatesModel::GetInstance().SubscribeListener(this);
}

void RecentPlatesMsgHandler::OnStopHandler()
{
    qDebug() << "RecentPlatesMsgHandler::OnStopHandler";
    RecentPlatesModel::GetInstance().UnSubscribeListener(this);
}

void RecentPlatesMsgHandler::OnRecentPlatesUpdated(const cv::Mat &frame, const QVector<QRect> &rectLists)
{
    if (rectLists.size() == 0)
        return;

    QRect rect = rectLists.at(0);

    if (rect.width() != 0 || rect.height() != 0) {
        QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
        QImage croppedImg = qImage.copy(rect);

        emit UpdateLaptopAppUi(croppedImg);
    }
}
