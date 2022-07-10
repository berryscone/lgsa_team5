#include "DebugInfoMsgHandler.h"

#include <QDebug>

DebugInfoMsgHandler::DebugInfoMsgHandler()
{

}

DebugInfoMsgHandler::~DebugInfoMsgHandler()
{

}

void DebugInfoMsgHandler::OnStartHandler()
{
    qDebug() << "DebugInfoMsgHandler::OnStartHandler";
    DebugInfoModel::GetInstance().SubscribeListener(this);
}

void DebugInfoMsgHandler::OnStopHandler()
{
    qDebug() << "DebugInfoMsgHandler::OnStopHandler";
    DebugInfoModel::GetInstance().UnSubscribeListener(this);
}

void DebugInfoMsgHandler::OnDebugInfoUpdated(const std::string &info)
{
    //NOTE : Do something in here

    emit UpdateLaptopAppUi(QString::fromStdString(info));
}
