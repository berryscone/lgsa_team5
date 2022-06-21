#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QPixmap>
#include "VideoSurface.h"
#include "ui_QtDemo.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::steady_clock::time_point;

class QtDemo : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtDemo(QWidget* parent = 0);
    ~QtDemo();

private slots:
    void onOpen();
    void onPause();
    void onResume();
    void onFrameUpdate(QPixmap pix);

private:
    Ui::QtDemoClass ui;
    QMediaPlayer mplayer;
    VideoSurface* vsurface;

    Clock clock;
    TimePoint prevTime;
};
