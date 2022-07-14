#pragma once

#include "IMsgHandler.h"
#include "model/AlertInfoModel.h"

#include <QImage>

class AlertInfoMsgHandler :
        public IMsgHandler,
        public IAlertInfoListener
{
    Q_OBJECT

public :
    AlertInfoMsgHandler();
    ~AlertInfoMsgHandler() override;

    // IMsgHandler interface
    void OnStartHandler() override;
    void OnStopHandler() override;

    // IAlertInfoListener interface
    void OnAlertInfoUpdated(const QImage &licensePlateImage, const std::string &alertInfo) override;

signals :
    void UpdateLaptopAppUi(const QImage &licensePlateImage, const QString &alertInfo);
};
