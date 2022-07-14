#pragma once
#include <string>
#include <functional>

#include <QtCore>

#include <opencv2/opencv.hpp>


using LoginCallback = std::function<void(const bool success, const QString detail)>;

class ILoginProvider abstract
{
public:
	virtual void RequestLogin(const QString id, const QString pw, LoginCallback) = 0;
};


class IVehicleQueryProvider : public QObject
{
	Q_OBJECT
public slots:
	virtual void RequestVehicleQuery(const cv::Mat plate_image, const QString plate_number) {
		throw std::logic_error("RequestVehicleQuery must be implemented by derived class!");
	}
};
