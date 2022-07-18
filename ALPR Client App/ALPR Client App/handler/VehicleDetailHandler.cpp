#include "VehicleDetailHandler.h"


VehicleDetailHandler& VehicleDetailHandler::GetInstance()
{
	static VehicleDetailHandler instance;
	return instance;
}

VehicleDetailHandler::VehicleDetailHandler()
{
	this->moveToThread(&mThread);
	mThread.start();
}

void VehicleDetailHandler::OnVehicleDetailProvided(const cv::Mat plate_image, const QJsonObject vehicle_detail)
{
	QImage plate_qimage(plate_image.data, plate_image.cols, plate_image.rows, plate_image.step, QImage::Format::Format_BGR888);

	QJsonArray vehicleDetailArray = vehicle_detail["vehicle_details"].toArray();

	if (vehicleDetailArray.size() > 0) {
		QJsonObject vehicleDetailJsonObject = vehicleDetailArray.at(0).toObject();
		QString licensePlatesNumber = vehicleDetailJsonObject["plate_number"].toString();

		if (licensePlatesNumber.compare(mLastPlateNumber) != 0) {
			emit SignalVehicleDetailPublish(plate_qimage.copy(), vehicle_detail);
			mLastPlateNumber = licensePlatesNumber;
		}
	}
}
