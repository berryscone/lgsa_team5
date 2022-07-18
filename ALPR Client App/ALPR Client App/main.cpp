#include "login/LoginWindow.h"
#include <QtWidgets/QApplication>


QTextStream log_stream;

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString curtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QByteArray localMsg = msg.toLocal8Bit();
    QFileInfo file_info(context.file ? context.file : "");
    const int line = context.line;

    QString stream;
    QTextStream msgStream(&stream);

    switch (type) {
    case QtDebugMsg:
        msgStream << "D ";
        break;
    case QtWarningMsg:
        msgStream << "W ";
        break;
    case QtCriticalMsg:
        msgStream << "C ";
        break;
    case QtFatalMsg:
        msgStream << "F ";
        break;
    case QtInfoMsg:
        msgStream << "I ";
        break;
    }
    msgStream << curtime << " " << localMsg << " (" << file_info.fileName() << ":" << line << ")" << endl;

    QString finalMsg = msgStream.readAll();
    log_stream << finalMsg;
    log_stream.flush();
    OutputDebugStringA(finalMsg.toStdString().c_str());
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
