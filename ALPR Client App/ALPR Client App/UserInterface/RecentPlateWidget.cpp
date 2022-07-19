#include "RecentPlateWidget.h"


RecentPlateWidget::RecentPlateWidget(QImage plateImage, QString requestPlate, bool isExact, QJsonObject plateDetail)
{
    mPlateDetail = plateDetail;

    mLabelPlateImage = new QLabel();
    mLabelPlateImage->setPixmap(QPixmap::fromImage(plateImage).scaled(140, 50));

    mLabelPlateRequest = new QLabel(requestPlate);
    mLabelPlateRequest->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QString responsePlate = plateDetail["plate_number"].toString();
    mLabelPlateResponse = new QLabel(responsePlate);
    mLabelPlateResponse->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    const QString status = plateDetail["status"].toString();
    const bool alert = status != "No Wants / Warrants";

    mLayout = new QHBoxLayout();
    mLayout->setContentsMargins(2, 2, 2, 2);
    mLayout->addWidget(mLabelPlateImage);
    mLayout->addWidget(mLabelPlateRequest);
    mLayout->addWidget(mLabelPlateResponse);

    if (isExact) {
        mLabelPlateResponse->setStyleSheet("QLabel {font-weight: bold; color: blue;}");
    }
    else {
        mLabelPlateResponse->setStyleSheet("QLabel {font-weight: bold;}");
    }

    if (alert) {
        setStyleSheet("RecentPlateWidget {border: 2px solid red;}");
    }
    else {
        setStyleSheet("RecentPlateWidget {border: 2px solid green;}");
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
