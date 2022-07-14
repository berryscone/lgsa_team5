#pragma once
#include <QtCore>
#include <QImage>

#include <opencv2/opencv.hpp>


class VehicleDetailHandler : public QObject
{
	Q_OBJECT
public:
	static VehicleDetailHandler& GetInstance();

signals:
	void SignalVehicleDetailPublish(const QImage plate_image, const QJsonObject vehicle_detail);

public slots:
	void OnVehicleDetailProvided(const cv::Mat plate_image, const QJsonObject vehicle_detail);

private:
	VehicleDetailHandler();

	QThread mThread;
};

