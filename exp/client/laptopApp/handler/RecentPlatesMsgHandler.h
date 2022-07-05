#ifndef RECENTPLATESMSGHANDLER_H
#define RECENTPLATESMSGHANDLER_H

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
    void OnRecentPlatesUpdated(const cv::Mat &frame, const QVector<QRect> &rectLists) override;

signals :
    void UpdateLaptopAppUi(QImage qImage);
};

#endif // RECENTPLATESMSGHANDLER_H
