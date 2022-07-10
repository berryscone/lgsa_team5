#include "AlprClientApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AlprClientApp w;
    w.show();
    return a.exec();
}
