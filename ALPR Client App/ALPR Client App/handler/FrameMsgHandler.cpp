#include "FrameMsgHandler.h"

#include <QDebug>

FrameMsgHandler::FrameMsgHandler()
{

}

FrameMsgHandler::~FrameMsgHandler()
{

}

void FrameMsgHandler::OnStartHandler()
{
    qDebug() << "FrameMsgHandler::OnStartHandler";
    FrameModel::GetInstance().SubscribeListener(this);
}

void FrameMsgHandler::OnStopHandler()
{
    qDebug() << "FrameMsgHandler::OnStopHandler";
    FrameModel::GetInstance().UnSubscribeListener(this);
}

void FrameMsgHandler::OnFrameUpdated(const QImage &frame)
{
    //NOTE : Do something in here

    emit UpdateLaptopAppUi(frame);
}
