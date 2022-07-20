#pragma once
#include <QtCore>


class QueryLogger
{
public:
	static QueryLogger& GetInstance();
	void LogRequest(const QString& plateNumber);
	void LogResponse(const QString& requestedPlateNumber, const QStringList& plateNumbers);

private:
	QueryLogger();
	inline uint GetUnixTime() { return QDateTime::currentDateTime().toTime_t(); }

	QFile mLogFile;
	QTextStream mLogStream;
};
