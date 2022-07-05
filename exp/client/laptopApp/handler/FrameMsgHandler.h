#ifndef FRAMEMSGHANDLER_H
#define FRAMEMSGHANDLER_H

#include "IMsgHandler.h"
#include "model/FrameModel.h"

#include <QImage>

class FrameMsgHandler :
        public IMsgHandler,
        public IFrameListener
{
    Q_OBJECT

public :
    FrameMsgHandler();
    ~FrameMsgHandler() override;

    // IMsgHandler interface
    void OnStartHandler() override;
    void OnStopHandler() override;

    // IFrameListener interface
public:
    void OnFrameUpdated(const QImage &frame) override;

signals :
    void UpdateLaptopAppUi(QImage frame);

private:
    QImage mFrame;
};

#endif // FRAMEMSGHANDLER_H
