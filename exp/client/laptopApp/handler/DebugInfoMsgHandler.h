#ifndef DEBUGINFOMSGHANDLER_H
#define DEBUGINFOMSGHANDLER_H

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

#endif // DEBUGINFOMSGHANDLER_H
