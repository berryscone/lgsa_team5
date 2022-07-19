#pragma once

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>
#include <QJsonObject>

class RecentPlateWidget : public QFrame
{
    Q_OBJECT

public:
    RecentPlateWidget(QImage plateImage, QString requestPlate, bool isExact, QJsonObject plateDetail);
    ~RecentPlateWidget();

    QPixmap GetLicensePlatePixmap() const;
    QString GetVehicleDetailInfo();

private:
    QLabel* mLabelPlateImage;
    QLabel* mLabelPlateRequest;
    QLabel* mLabelPlateResponse;
    QHBoxLayout *mLayout;
    
    QJsonObject mPlateDetail;
};
