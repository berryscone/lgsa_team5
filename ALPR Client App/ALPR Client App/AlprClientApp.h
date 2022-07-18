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
    void InitFrameGenerator();

signals:
    void startFrameGenerator();
    void pauseFrameGenerator();
    void resumeFrameGenerator();
    void stopFrameGenerator();

private slots:
    void OnOpen();
    void OnStop();
    void OnToggle(bool bIsPause);
    void OnRecentPlatesViewItemClicked(QListWidgetItem* item);
    void OnVideoStopped();

    void UpdateUI(const QImage plate_image, const QJsonObject vehicle_detail);
    void UpdatePlaybackView(QPixmap pixmap);
    void UpdateDebugInfoView(QString debugInfo);
    void UpdateNetworkStatusUI(QNetworkReply::NetworkError status);

private:
    void UpdateRecentPlatesView(QImage &licensePlateImage, QJsonObject &vehicleDetailJsonObject);
    void UpdateVehicleInfoView(QImage &licensePlateImage, QJsonObject &vehicleDetailJsonObject);
    void UpdateAlertInfoView(QImage &licensePlateImage, QJsonObject &vehicleDetailJsonObject);

    void SetControllerRun();
    void SetControllerStop();
    void SetToggleButtonToPause();
    void SetToggleButtonToPlay();

    void SetFrameGeneratorButtonStyle();

    Ui::AlprClientAppClass *ui;

    QThread mFrameGeneratorThread;
    FrameGenerator mFrameGenerator;
    bool mbIsStart;

    QGraphicsPixmapItem mPlaybackPixmap;
    QGraphicsPixmapItem mRecentPlatesPixmap;
    std::string mFilePath;

    MsgHandlerManagerPtr mMsgHandlerManager;
    std::unique_ptr<DebugInfoMsgHandler> mDebugInfoMsgHandler;

    int mLicensePlateImageWidth;
    int mLicensePlateImageHeight;
};
