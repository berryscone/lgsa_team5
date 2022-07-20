#pragma once

#include <list>
#include <QtCore/qstring.h>

#define MAX_NUM_OF_LIST 20

using namespace std;

class DuplicateQueryResultChecker
{
public:
	bool isDuplicate(QString plateNum);
	void addQueryResult(QString plateNum);

private:
	list<QString> mListQueryResult;
};

