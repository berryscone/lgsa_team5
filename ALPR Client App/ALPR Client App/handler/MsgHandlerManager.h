#pragma once

#include "IMsgHandler.h"

#include <vector>
#include <memory>

#include <QImage>

class MsgHandlerManager : public QObject
{
    Q_OBJECT

public:
    MsgHandlerManager();
    ~MsgHandlerManager() = default;

    void Start();
    void Stop();
    void AddMsgHandler(IMsgHandler* pMsgHandler);

private:
    std::vector<IMsgHandler*> mMsgHandlerVector;
};

using MsgHandlerManagerPtr = std::shared_ptr<MsgHandlerManager>;
