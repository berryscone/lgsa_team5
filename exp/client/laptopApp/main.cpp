#include "LaptopApp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LaptopApp w;
    w.show();
    return a.exec();
}
