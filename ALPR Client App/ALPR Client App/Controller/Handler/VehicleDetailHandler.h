#pragma once
#include <opencv2/opencv.hpp>

#include "Controller/QueryLogger.h"
#include "Model/VehicleDetail.h"


class VehicleDetailHandler : public QObject
{
	Q_OBJECT
public:
	static VehicleDetailHandler& GetInstance();

signals:
	void SignalVehicleDetailPublish(VehicleDetail vehicleDetail);

public slots:
	void OnVehicleDetailProvided(const cv::Mat plate_image, const QJsonObject vehicle_detail);

private:
	VehicleDetailHandler();

	QThread mThread;
	QString mLastPlateNumber;
	QueryLogger& mQueryLogger;
};

