#include "LaptopApp.h"
#include "ui_LaptopApp.h"

LaptopApp::LaptopApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LaptopApp)
{
    ui->setupUi(this);

    ui->playbackView->setScene(new QGraphicsScene(this));
    ui->playbackView->scene()->addItem(&mPlaybackPixmap);
    ui->vehicleInfo->setWordWrap(true);

    connect(ui->openButton, &QPushButton::clicked, this, &LaptopApp::onOpen);
    connect(ui->startButton, &QPushButton::clicked, this, &LaptopApp::onStart);
    connect(ui->stopButton, &QPushButton::clicked, this, &LaptopApp::onStop);
    connect(ui->pauseButton, &QPushButton::clicked, this, &LaptopApp::onPause);
    connect(ui->resumeButton, &QPushButton::clicked, this, &LaptopApp::onResume);

    mMsgHandlerManager = std::make_unique<MsgHandlerManager>();
    mFrameMsgHandler = std::make_unique<FrameMsgHandler>();
    mRecentPlatesMsgHandler = std::make_unique<RecentPlatesMsgHandler>();
    mVehicleInfoMsgHandler = std::make_unique<VehicleInfoMsgHandler>();
    mDebugInfoMsgHandler = std::make_unique<DebugInfoMsgHandler>();

    mMsgHandlerManager->AddMsgHandler(mFrameMsgHandler.get());
    mMsgHandlerManager->AddMsgHandler(mRecentPlatesMsgHandler.get());
    mMsgHandlerManager->AddMsgHandler(mVehicleInfoMsgHandler.get());
    mMsgHandlerManager->AddMsgHandler(mDebugInfoMsgHandler.get());

    mFrameGenerator = std::make_unique<FrameGenerator>();

    connect(mFrameMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QImage)),
            this, SLOT(UpdatePlaybackView(QImage)));

    connect(mRecentPlatesMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QImage)),
            this, SLOT(UpdateRecentPlatesView(QImage)));

    connect(mVehicleInfoMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QString)),
            this, SLOT(UpdateVehicleInfoView(QString)));

    connect(mDebugInfoMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QString)),
            this, SLOT(UpdateDebugInfoView(QString)));
}

void LaptopApp::UpdatePlaybackView(QImage frame)
{   
    mPlaybackPixmap.setPixmap(QPixmap::fromImage(frame));
    ui->playbackView->fitInView(&mPlaybackPixmap, Qt::KeepAspectRatio);

    qApp->processEvents();
}

void LaptopApp::UpdateRecentPlatesView(QImage frame)
{
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap(QPixmap::fromImage(frame)));
    QListWidgetItem *plate = new QListWidgetItem;
    auto size = label->sizeHint();
    plate->setSizeHint(label->sizeHint());
    size = plate->sizeHint();
    ui->recentPlatesListView->addItem(plate);
    ui->recentPlatesListView->setItemWidget(plate, label);
    ui->recentPlatesListView->setSpacing(3);
    ui->recentPlatesListView->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    ui->recentPlatesListView->scrollToBottom();
    ui->recentPlatesListView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->recentPlatesListView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
}

void LaptopApp::UpdateVehicleInfoView(QString info)
{
    ui->vehicleInfo->setText(info);
    ui->vehicleInfo->setWordWrap(true);
}

void LaptopApp::UpdateDebugInfoView(QString info)
{
    ui->debugInfo->setText(info);
}

void LaptopApp::onOpen()
{
#if 0
    mFilePath = "D:/hjh_world/CMU/StudioProject/beaver1.avi";
#else
    QFileDialog fd;
    fd.show();
    fd.exec();
    QString strFileName = fd.selectedFiles().at(0);
    mFilePath = strFileName.toStdString();
#endif

    qDebug() << "call onOpen tid:" << QThread::currentThreadId() << ", mFilePath:" << mFilePath.data();
}

void LaptopApp::onStart()
{
    qDebug() << "call onStart tid:" << QThread::currentThreadId();

    mFrameGenerator->Start(mFilePath);
    mMsgHandlerManager->Start();
}

void LaptopApp::onStop()
{
    mFrameGenerator->Stop();
    mMsgHandlerManager->Stop();

    QApplication::quit();
}

void LaptopApp::onPause()
{
    mFrameGenerator->Pause();
    mMsgHandlerManager->Stop();
}

void LaptopApp::onResume()
{
    mFrameGenerator->Resume();
    mMsgHandlerManager->Start();
}

void LaptopApp::closeEvent(QCloseEvent *event)
{
    qDebug() << "call closeEvent tid:" << QThread::currentThreadId();

    mFrameGenerator->Stop();
    mMsgHandlerManager->Stop();
}

LaptopApp::~LaptopApp()
{
    delete ui;
}

