#include "RecentPlateWidget.h"


RecentPlateWidget::RecentPlateWidget(const VehicleDetail& vehicleDetail)
    : mVehicleDetail(mVehicleDetail)
{
    mLabelPlateImage = new QLabel();
    mLabelPlateImage->setPixmap(QPixmap::fromImage(vehicleDetail.image).scaled(140, 50));

    mLabelPlateRequest = new QLabel(vehicleDetail.requestNumber);

    if (vehicleDetail.exactMatch) {
        mLabelPlateRequest->setText("");
    }
    mLabelPlateRequest->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    mLabelPlateResponse = new QLabel(vehicleDetail.number);
    mLabelPlateResponse->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mLabelPlateResponse->setStyleSheet("QLabel {font-weight: bold;}");

    mLayout = new QHBoxLayout();
    mLayout->setContentsMargins(2, 2, 2, 2);
    mLayout->addWidget(mLabelPlateImage);
    mLayout->addWidget(mLabelPlateRequest);
    mLayout->addWidget(mLabelPlateResponse);

    if (vehicleDetail.status == VehicleDetail::Status::Normal) {
        setStyleSheet("RecentPlateWidget {border: 2px solid green;}");
    }
    else {
        setStyleSheet("RecentPlateWidget {border: 2px solid red;}");
    }

    setLayout(mLayout);
}

RecentPlateWidget::~RecentPlateWidget()
{
    delete mLabelPlateImage;
    delete mLayout;
}

QPixmap RecentPlateWidget::GetLicensePlatePixmap() const
{
    return *mLabelPlateImage->pixmap();
}

QString RecentPlateWidget::GetVehicleDetailInfo()
{
    return mLabelPlateRequest->text();
}
