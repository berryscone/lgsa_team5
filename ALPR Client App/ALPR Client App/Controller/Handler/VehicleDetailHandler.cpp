#include "VehicleDetailHandler.h"


VehicleDetailHandler& VehicleDetailHandler::GetInstance()
{
	static VehicleDetailHandler instance;
	return instance;
}

VehicleDetailHandler::VehicleDetailHandler() :
	mQueryLogger(QueryLogger::GetInstance())
{
	this->moveToThread(&mThread);
	mThread.start();
}

void VehicleDetailHandler::OnVehicleDetailProvided(const cv::Mat plate_image, const QJsonObject vehicle_detail)
{
	const QString requestNumber = vehicle_detail["plate_number"].toString();
	const bool exact = vehicle_detail["is_exact"].toBool();
	QJsonArray vehicleDetailArray = vehicle_detail["vehicle_details"].toArray();

	QStringList responseList;
	for (int i = 0; i < vehicleDetailArray.size(); ++i) {
		if (i > 0 && !exact) {
			break;
		}

		QJsonObject vehicleDetailJsonObject = vehicleDetailArray.at(i).toObject();
		VehicleDetail vehicleDetail = VehicleDetail::Create(plate_image, requestNumber, exact, vehicleDetailJsonObject);
		responseList.append(vehicleDetail.number);
		
		if (vehicleDetail.number != mLastPlateNumber) {
			qDebug() << vehicleDetail.requestNumber << "=>" << vehicleDetail.number << exact;
			mLastPlateNumber = vehicleDetail.number;
			emit SignalVehicleDetailPublish(vehicleDetail);
		}
	}

	mQueryLogger.LogResponse(responseList);
}
