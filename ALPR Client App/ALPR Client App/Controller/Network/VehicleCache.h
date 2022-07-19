#pragma once

#include <list>
#include <map>
#include <string>

#include <QtCore/qjsonobject.h>

using namespace std;

#define MAX_NUM_OF_CACHE 1000

class VehicleCache
{
public:
	bool isRequestedQuery(QString plateNum);
	void addRequestedList(QString plateNum);

	void putQueryResult(QString plateNum, QJsonObject vehicleInfo);
	QJsonObject getQueryResult(QString plateNum);

private:
	void handleWhenCacheIsFull();

	list<QString> mListRequestedQuery;
	map<QString, QJsonObject> mMapVehicleInfo;
};
