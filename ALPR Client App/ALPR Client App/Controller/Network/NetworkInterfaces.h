#pragma once
#include <string>
#include <functional>

#include <QtCore>

#include <opencv2/opencv.hpp>


class ILoginProvider abstract
{
public:
	virtual void RequestLogin(const QString id, const QString pw, LoginCallback) = 0;
};


class IVehicleQueryProvider : public QObject
{
	Q_OBJECT
public slots:
	virtual void RequestVehicleQuery(const cv::Mat plate_image, const QString plate_number, const int retry_cnt = 0) {
		throw std::logic_error("RequestVehicleQuery must be implemented by derived class!");
	}
};
