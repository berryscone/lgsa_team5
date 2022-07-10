#pragma once

#include "IMsgHandler.h"
#include "model/DebugInfoModel.h"

#include <QImage>

class DebugInfoMsgHandler :
        public IMsgHandler,
        public IDebugInfoListener
{
    Q_OBJECT

public :
    DebugInfoMsgHandler();
    ~DebugInfoMsgHandler() override;

    // IMsgHandler interface
    void OnStartHandler() override;
    void OnStopHandler() override;

    // IDebugInfoListener interface
    void OnDebugInfoUpdated(const std::string &info) override;

signals :
    void UpdateLaptopAppUi(QString info);
};
