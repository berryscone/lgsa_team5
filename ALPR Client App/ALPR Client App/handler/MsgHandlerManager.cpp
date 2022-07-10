#include "MsgHandlerManager.h"

MsgHandlerManager::MsgHandlerManager()
{

}

void MsgHandlerManager::Start()
{
    for (auto* handler : mMsgHandlerVector) {
        handler->OnStartHandler();
    }
}

void MsgHandlerManager::Stop()
{
    for (auto* handler : mMsgHandlerVector) {
        handler->OnStopHandler();
    }
}

void MsgHandlerManager::AddMsgHandler(IMsgHandler *pMsgHandler)
{
    mMsgHandlerVector.push_back(pMsgHandler);
}
