#pragma once

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>
#include <QJsonObject>

#include "Model/VehicleDetail.h"


class RecentPlateWidget : public QFrame
{
    Q_OBJECT

public:
    RecentPlateWidget(const VehicleDetail& vehicleDetail);
    ~RecentPlateWidget();

    QPixmap GetLicensePlatePixmap() const;
    const VehicleDetail& GetVehicleDetail() { return mVehicleDetail; };

private:
    QLabel* mLabelPlateImage;
    QLabel* mLabelPlateRequest;
    QLabel* mLabelPlateResponse;
    QHBoxLayout *mLayout;
    
    const VehicleDetail mVehicleDetail;
};
