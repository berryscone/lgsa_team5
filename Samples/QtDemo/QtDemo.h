#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtDemo.h"

class QtDemo : public QMainWindow
{
    Q_OBJECT

public:
    QtDemo(QWidget *parent = nullptr);
    ~QtDemo();

private:
    Ui::QtDemoClass ui;
};
