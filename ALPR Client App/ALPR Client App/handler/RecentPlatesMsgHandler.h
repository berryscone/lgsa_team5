#pragma once

#include "IMsgHandler.h"
#include "model/RecentPlatesModel.h"

#include <QImage>

#include <opencv2/opencv.hpp>

class RecentPlatesMsgHandler :
        public IMsgHandler,
        public IRecentPlatesListener
{
    Q_OBJECT

public :
    RecentPlatesMsgHandler();
    ~RecentPlatesMsgHandler() override;

    // IMsgHandler interface
    void OnStartHandler() override;
    void OnStopHandler() override;

    // IRecentPlatesListener interface
    void OnRecentPlatesUpdated(const QImage &licensePlateImage, const QString &vehicleInfo) override;

signals :
    void UpdateLaptopAppUi(QImage licensePlateImage, QString vehicleInfo);
};
