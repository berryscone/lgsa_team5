#include "login/LoginWindow.h"
#include <QtWidgets/QApplication>


QTextStream log_stream;

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QByteArray localMsg = msg.toLocal8Bit();
    QFileInfo file_info(context.file ? context.file : "");
    const int line = context.line;

    switch (type) {
    case QtDebugMsg:
        log_stream << "D " << curtime << localMsg << " (" << file_info.fileName() << ":" << line << ")" << endl;
        break;
    case QtWarningMsg:
        log_stream << "W " << curtime << localMsg << " (" << file_info.fileName() << ":" << line << ")" << endl;
        break;
    case QtCriticalMsg:
        log_stream << "C " << curtime << localMsg << " (" << file_info.fileName() << ":" << line << ")" << endl;
        break;
    case QtFatalMsg:
        log_stream << "F " << curtime << localMsg << " (" << file_info.fileName() << ":" << line << ")" << endl;
        break;
    case QtInfoMsg:
        log_stream << "I " << curtime << localMsg << " (" << file_info.fileName() << ":" << line << ")" << endl;
        break;
    }
    log_stream.flush();
}

int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>("cv::Mat");

    QFile log_file("Alpr.log");
    if (!log_file.open(QIODevice::Append | QIODevice::Text)) {
        qCritical() << "Unable to open Alpr.log";
        return -1;
    }
    log_stream.setDevice(&log_file);
    qInstallMessageHandler(myMessageOutput);
    qInfo() << "--------- ALPR Start ---------";

    QApplication a(argc, argv);
    LoginWindow w;
    w.show();
    return a.exec();
}
