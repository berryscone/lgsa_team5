#include "RecentPlateCustomWidget.h"

RecentPlateCustomWidget::RecentPlateCustomWidget(QImage licensePlateImage, QString vehicleDetailInfo)
{
    mLicensePlateImageLabel = new QLabel();
    mVehicleDetailInfoLabel = new QLabel(vehicleDetailInfo);

    mLicensePlateImageLabel->setPixmap(QPixmap(QPixmap::fromImage(licensePlateImage)));
    mLicensePlateImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mLicensePlateImageLabel->setFixedWidth(140);
    mLicensePlateImageLabel->setFixedHeight(50);

    mVehicleDetailInfoLabel->hide();

    mLayout = new QHBoxLayout();
    mLayout->addWidget(mLicensePlateImageLabel);
    mLayout->addWidget(mVehicleDetailInfoLabel);
    mLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    setLayout(mLayout);
}

RecentPlateCustomWidget::~RecentPlateCustomWidget()
{
    delete mLicensePlateImageLabel;
    delete mVehicleDetailInfoLabel;
    delete mLayout;
}

QPixmap RecentPlateCustomWidget::GetLicensePlatePixmap() const
{
    return *mLicensePlateImageLabel->pixmap();
}

QString RecentPlateCustomWidget::GetVehicleDetailInfo()
{
    return mVehicleDetailInfoLabel->text();
}
