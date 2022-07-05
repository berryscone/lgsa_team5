#ifndef LAPTOPAPP_H
#define LAPTOPAPP_H

#include "handler/MsgHandlerManager.h"
#include "handler/FrameMsgHandler.h"
#include "handler/RecentPlatesMsgHandler.h"
#include "handler/VehicleInfoMsgHandler.h"
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
namespace Ui { class LaptopApp; }
QT_END_NAMESPACE

class LaptopApp : public QMainWindow
{
    Q_OBJECT

public:
    LaptopApp(QWidget *parent = nullptr);
    ~LaptopApp();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void onOpen();
    void onStart();
    void onStop();
    void onPause();
    void onResume();

    void UpdatePlaybackView(QImage frame);
    void UpdateRecentPlatesView(QImage frame);
    void UpdateVehicleInfoView(QString info);
    void UpdateDebugInfoView(QString info);

private:
    Ui::LaptopApp *ui;

    QGraphicsPixmapItem mPlaybackPixmap;
    QGraphicsPixmapItem mRecentPlatesPixmap;
    std::string mFilePath;

    MsgHandlerManagerPtr mMsgHandlerManager;
    std::unique_ptr<FrameMsgHandler> mFrameMsgHandler;
    std::unique_ptr<RecentPlatesMsgHandler> mRecentPlatesMsgHandler;
    std::unique_ptr<VehicleInfoMsgHandler> mVehicleInfoMsgHandler;
    std::unique_ptr<DebugInfoMsgHandler> mDebugInfoMsgHandler;

    std::unique_ptr<FrameGenerator> mFrameGenerator;
};
#endif // LAPTOPAPP_H
