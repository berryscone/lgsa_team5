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
	inline qint64 GetUnixTime() { return QDateTime::currentMSecsSinceEpoch(); }

	QFile mLogFile;
	QTextStream mLogStream;
};
