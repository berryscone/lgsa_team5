#pragma once

#include <memory>

#include <QtWidgets>
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
#include <QtNetwork>
#include "ui_AlprClientApp.h"

#include "Controller/Alpr/FrameGenerator.h"
#include "Controller/Handler/VehicleDetailHandler.h"
#include "View/VehicleDetailDialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class AlprClientAppClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    void OnToggle(bool checked);
    void OnRecentPlatesViewItemClicked(QListWidgetItem* item);
    void OnVideoStopped();

    void UpdateUI(const VehicleDetail vehicleDetail);
    void UpdatePlaybackView(QPixmap pixmap);
    void UpdateDebugInfoView(QString debugInfo);
    void UpdateNetworkStatusUI(QNetworkReply::NetworkError status);

private:
    void UpdateRecentPlatesView(const VehicleDetail& vehicleDetail);
    void UpdateVehicleInfoView(const VehicleDetail& vehicleDetail);
    void UpdateAlertInfoView(const VehicleDetail& vehicleDetail);

    void SetControllerRun();
    void SetControllerStop();
    void SetToggleButtonToPause();
    void SetToggleButtonToPlay();

    void SetFrameGeneratorButtonStyle();

    Ui::AlprClientAppClass *ui;

    QThread mFrameGeneratorThread;
    FrameGenerator mFrameGenerator;
    VehicleDetailDialog mVehicleDetailDialog;

    QGraphicsPixmapItem mPlaybackPixmap;
    QGraphicsPixmapItem mRecentPlatesPixmap;
    std::string mFilePath;

    QPixmap mIconNetIndicatorRed;
    QPixmap mIconNetIndicatorGreen;

    const int mLicensePlateImageWidth = 140;
    const int mLicensePlateImageHeight = 50;
};
