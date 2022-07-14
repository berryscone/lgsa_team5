#include "login/LoginWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    QApplication a(argc, argv);
    LoginWindow w;
    w.show();
    return a.exec();
}
