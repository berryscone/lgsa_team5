#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AlprClientApp.h"

#include "handler/MsgHandlerManager.h"
#include "handler/DebugInfoMsgHandler.h"
#include "generator/FrameGenerator.h"

#include <memory>

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QThread>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QVector>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class AlprClientAppClass; };
QT_END_NAMESPACE

class AlprClientApp : public QMainWindow
{
    Q_OBJECT

public:
    AlprClientApp(QWidget *parent = nullptr);
    ~AlprClientApp();

protected:
    void closeEvent(QCloseEvent* event);

private:
    void makeFrameGeneratorThread();

signals:
    void startFrameGenerator();
    void pauseFrameGenerator();
    void resumeFrameGenerator();
    void stopFrameGenerator();

private slots:
    void onOpen();
    void onToggle(bool bIsPause);
    void OnRecentPlatesViewItemClicked(QListWidgetItem* item);

    void UpdateUI(const QImage plate_image, const QJsonObject vehicle_detail);
    void UpdatePlaybackView(QPixmap pixmap);
    void UpdateDebugInfoView(QString debugInfo);

private:
    void UpdateRecentPlatesView(QImage licensePlateImage, QString vehicleInfo);
    void UpdateVehicleInfoView(QString vehicleInfo);
    void UpdateAlertInfoView(QImage licensePlateImage, QString alertInfo);

    Ui::AlprClientAppClass *ui;
    QThread *mFrameGeneratorThread;
    bool mbIsStart;

    QGraphicsPixmapItem mPlaybackPixmap;
    QGraphicsPixmapItem mRecentPlatesPixmap;
    QGraphicsPixmapItem mAlertLicensePlatePixmap;
    std::string mFilePath;

    MsgHandlerManagerPtr mMsgHandlerManager;
    std::unique_ptr<DebugInfoMsgHandler> mDebugInfoMsgHandler;
    std::unique_ptr<FrameGenerator> mFrameGenerator;
};
