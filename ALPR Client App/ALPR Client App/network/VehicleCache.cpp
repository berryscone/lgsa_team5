#include "VehicleCache.h"

bool VehicleCache::isRequestedQuery(QString plateNum)
{
	std::list<QString>::iterator it =
		std::find(
			mListRequestedQuery.begin(),
			mListRequestedQuery.end(),
			plateNum);

	if (it != mListRequestedQuery.end()) {
		return true;
	} else {
		return false;
	}
}

void VehicleCache::addRequestedList(QString plateNum)
{
	handleWhenCacheIsFull();
	mListRequestedQuery.push_back(plateNum);
}

void VehicleCache::handleWhenCacheIsFull()
{
	if (mListRequestedQuery.size() == MAX_NUM_OF_CACHE) {
		mMapVehicleInfo.erase(mListRequestedQuery.front());
		mListRequestedQuery.pop_front();
	}
}

void VehicleCache::putQueryResult(QString plateNum, QJsonObject vehicleInfo)
{
	mMapVehicleInfo.insert({ plateNum, vehicleInfo });
}

QJsonObject VehicleCache::getQueryResult(QString plateNum)
{
	if (mMapVehicleInfo.find(plateNum) != mMapVehicleInfo.end()) {
		return mMapVehicleInfo.at(plateNum);
	} else {
		return QJsonObject();
	}
}
