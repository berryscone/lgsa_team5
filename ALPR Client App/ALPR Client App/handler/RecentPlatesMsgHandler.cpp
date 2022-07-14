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

void RecentPlatesMsgHandler::OnRecentPlatesUpdated(const QImage &licensePlateImage, const QString &vehicleInfo)
{
    emit UpdateLaptopAppUi(licensePlateImage, vehicleInfo);
}
