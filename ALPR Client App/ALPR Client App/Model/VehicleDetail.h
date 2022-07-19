#pragma once
#include <unordered_map>

#include <QtCore>
#include <QImage>

#include <opencv2/opencv.hpp>


class VehicleDetail
{
public:
	enum Status {
		Normal,
		OwnerWanted,
		UnpaidFines,
		Stolen,
		NumStatus
	};
	static const std::unordered_map<QString, Status> StrToStatus;

	QImage image;
	bool exactMatch;
	QString requestNumber;

	QString rawStatus;
	Status status;
	QString number;
	QString owner;
	QString birth;
	QString address;
	QString make;
	QString model;
	QString year;
	QString color;

	VehicleDetail() = default;
	VehicleDetail(const VehicleDetail& other);
	static VehicleDetail Create(cv::Mat image, QString requestNumber, bool exact, const QJsonObject detail);
};
