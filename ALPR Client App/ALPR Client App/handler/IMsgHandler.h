#pragma once

#include <QObject>
#include <QImage>

class IMsgHandler : public QObject
{
    Q_OBJECT

public:
    virtual ~IMsgHandler() = default;

    virtual void OnStartHandler() = 0;
    virtual void OnStopHandler() = 0;
};
