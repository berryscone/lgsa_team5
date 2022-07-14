#include "AlertInfoMsgHandler.h"

#include <QDebug>

AlertInfoMsgHandler::AlertInfoMsgHandler()
{

}

AlertInfoMsgHandler::~AlertInfoMsgHandler()
{

}

void AlertInfoMsgHandler::OnStartHandler()
{
    qDebug() << "AlertInfoMsgHandler::OnStartHandler";
    AlertInfoModel::GetInstance().SubscribeListener(this);
}

void AlertInfoMsgHandler::OnStopHandler()
{
    qDebug() << "AlertInfoMsgHandler::OnStopHandler";
    AlertInfoModel::GetInstance().UnSubscribeListener(this);
}

void AlertInfoMsgHandler::OnAlertInfoUpdated(const QImage &licensePlateImage, const std::string &alertInfo)
{
    //NOTE : Do something in here

    emit UpdateLaptopAppUi(licensePlateImage, QString::fromStdString(alertInfo));
}
