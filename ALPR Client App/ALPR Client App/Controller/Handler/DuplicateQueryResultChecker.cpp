#include "DuplicateQueryResultChecker.h"

bool DuplicateQueryResultChecker::isDuplicate(QString plateNum)
{
	std::list<QString>::iterator it =
		std::find(
			mListQueryResult.begin(),
			mListQueryResult.end(),
			plateNum);

	if (it != mListQueryResult.end()) {
		return true;
	} else {
		return false;
	}
}

void DuplicateQueryResultChecker::addQueryResult(QString plateNum)
{
	if (mListQueryResult.size() == MAX_NUM_OF_LIST) {
		mListQueryResult.pop_front();
	}
	mListQueryResult.push_back(plateNum);
}
