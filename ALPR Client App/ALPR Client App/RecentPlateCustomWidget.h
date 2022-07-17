#pragma once

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>

class RecentPlateCustomWidget : public QWidget
{
    Q_OBJECT

public:
    RecentPlateCustomWidget(QImage licensePlateImage, QString vehicleDetailInfo);
    ~RecentPlateCustomWidget();

    QPixmap GetLicensePlatePixmap() const;
    QString GetVehicleDetailInfo();

private:
    QLabel *mLicensePlateImageLabel;
    QLabel *mVehicleDetailInfoLabel;
    QHBoxLayout *mLayout;
};
