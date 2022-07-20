#include "QueryLogger.h"


QueryLogger& QueryLogger::GetInstance() {
    static QueryLogger instance;
    return instance;
}

void QueryLogger::LogRequest(const QString& plateNumber) {
    mLogStream << "REQUEST  " << GetUnixTime() << " " << plateNumber << endl;
}

void QueryLogger::LogResponse(const QString& requestedPlateNumber, const QStringList& plateNumbers) {
    mLogStream << "RESPONSE " << GetUnixTime() << " " << requestedPlateNumber << " =>";
    for (int i = 0; i < plateNumbers.size(); ++i) {
        mLogStream << " " << plateNumbers.at(i);
    }
    mLogStream << endl;
}

QueryLogger::QueryLogger() {
    mLogFile.setFileName("AlprQuery.log");
    if (mLogFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        mLogStream.setDevice(&mLogFile);
    }
}
