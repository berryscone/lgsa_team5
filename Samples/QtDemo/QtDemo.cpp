#include "QtDemo.h"
#include <qdebug.h>

QtDemo::QtDemo(QWidget* parent) :
    QMainWindow(parent), mplayer(parent, QMediaPlayer::VideoSurface)
{
    ui.setupUi(this);
    connect(ui.pushOpen, &QPushButton::clicked, this, &QtDemo::onOpen);
    connect(ui.pushPause, &QPushButton::clicked, this, &QtDemo::onPause);
    connect(ui.pushResume, &QPushButton::clicked, this, &QtDemo::onResume);
}

QtDemo::~QtDemo()
{
    if (vsurface) {
        vsurface->stop();
        vsurface->deleteLater();
    }
    mplayer.stop();
}

void QtDemo::onOpen()
{
    // Select video file
    QFileDialog fd;
    fd.show();
    fd.exec();
    QString strFileName = fd.selectedFiles().at(0);

    // Create Video Surface
    // 새로운 비디오 열었을 때 메모리 관리 어떻게 할지??
    vsurface = new VideoSurface();
    mplayer.setVideoOutput(vsurface);

    connect(vsurface, SIGNAL(imageProcesssed(QPixmap)),
        this, SLOT(onFrameUpdate(QPixmap)), Qt::QueuedConnection);

    mplayer.setMedia(QUrl::fromLocalFile(strFileName));
    mplayer.play();
}

void QtDemo::onPause()
{
    mplayer.pause();
}

void QtDemo::onResume()
{
    mplayer.play();
}

void QtDemo::onFrameUpdate(QPixmap pix)
{
    TimePoint currTime = clock.now();
    auto count = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count();
    prevTime = currTime;

    QString strCount;
    strCount.sprintf("%lld ms", count);

    ui.labelInfo->setText(strCount);
    ui.labelFrame->setPixmap(pix);
    ui.labelFrame->update();
}
