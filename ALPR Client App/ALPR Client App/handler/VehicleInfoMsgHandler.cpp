#include "VehicleInfoMsgHandler.h"

#include <QDebug>

VehicleInfoMsgHandler::VehicleInfoMsgHandler()
{

}

VehicleInfoMsgHandler::~VehicleInfoMsgHandler()
{

}

void VehicleInfoMsgHandler::OnStartHandler()
{
    qDebug() << "VehicleInfoMsgHandler::OnStartHandler";
    VehicleInfoModel::GetInstance().SubscribeListener(this);
}

void VehicleInfoMsgHandler::OnStopHandler()
{
    qDebug() << "VehicleInfoMsgHandler::OnStopHandler";
    VehicleInfoModel::GetInstance().UnSubscribeListener(this);
}

void VehicleInfoMsgHandler::OnVehicleInfoUpdated(const std::string &info)
{
    //NOTE : Do something in here

    emit UpdateLaptopAppUi(QString::fromStdString(info));
}
