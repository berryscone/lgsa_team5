#include "VehicleDetail.h"


const std::unordered_map<QString, VehicleDetail::Status> VehicleDetail::StrToStatus = {
	{ "No Wants / Warrants", VehicleDetail::Normal },
	{ "Owner Wanted", VehicleDetail::OwnerWanted },
	{ "Unpaid Fines - Tow", VehicleDetail::UnpaidFines },
	{ "Stolen", VehicleDetail::Normal },
};

VehicleDetail::VehicleDetail(const VehicleDetail& other) {
	image = other.image.copy();
	exactMatch = other.exactMatch;
	requestNumber = other.requestNumber;

	rawStatus = other.rawStatus;
	status = other.status;
	number = other.number;
	owner = other.owner;
	birth = other.birth;
	address = other.address;
	make = other.make;
	model = other.model;
	year = other.year;
	color = other.color;
}

VehicleDetail VehicleDetail::Create(cv::Mat image, QString requestNumber, bool exact, const QJsonObject detail) {
	VehicleDetail vd;
	vd.image = QImage(image.data, image.cols, image.rows, image.step, QImage::Format::Format_BGR888).copy();
	vd.exactMatch = exact;
	vd.requestNumber = requestNumber;

	vd.rawStatus = detail["status"].toString();
	vd.status = StrToStatus.at(vd.rawStatus);
	vd.number = detail["plate_number"].toString();
	vd.owner = detail["owner"].toString();
	vd.birth = detail["birth"].toString();
	vd.address = detail["address"].toString();
	vd.make = detail["make"].toString();
	vd.model = detail["model"].toString();
	vd.year = detail["year"].toString();
	vd.color = detail["color"].toString();
	return vd;
}
