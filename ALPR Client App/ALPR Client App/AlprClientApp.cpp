#include "AlprClientApp.h"

AlprClientApp::AlprClientApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlprClientAppClass())
{
    ui->setupUi(this);

    ui->playbackView->setScene(new QGraphicsScene(this));
    ui->playbackView->scene()->addItem(&mPlaybackPixmap);
    ui->vehicleInfo->setWordWrap(true);

    connect(ui->openButton, &QPushButton::clicked, this, &AlprClientApp::onOpen);
    connect(ui->startButton, &QPushButton::clicked, this, &AlprClientApp::onStart);
    connect(ui->stopButton, &QPushButton::clicked, this, &AlprClientApp::onStop);
    connect(ui->pauseButton, &QPushButton::clicked, this, &AlprClientApp::onPause);
    connect(ui->resumeButton, &QPushButton::clicked, this, &AlprClientApp::onResume);

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

void AlprClientApp::UpdatePlaybackView(QImage frame)
{
    mPlaybackPixmap.setPixmap(QPixmap::fromImage(frame));
    ui->playbackView->fitInView(&mPlaybackPixmap, Qt::KeepAspectRatio);

    qApp->processEvents();
}

void AlprClientApp::UpdateRecentPlatesView(QImage frame)
{
    QLabel* label = new QLabel;
    label->setPixmap(QPixmap(QPixmap::fromImage(frame)));
    QListWidgetItem* plate = new QListWidgetItem;
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

void AlprClientApp::UpdateVehicleInfoView(QString info)
{
    ui->vehicleInfo->setText(info);
    ui->vehicleInfo->setWordWrap(true);
}

void AlprClientApp::UpdateDebugInfoView(QString info)
{
    ui->debugInfo->setText(info);
}

void AlprClientApp::onOpen()
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

void AlprClientApp::onStart()
{
    qDebug() << "call onStart tid:" << QThread::currentThreadId();

    mFrameGenerator->Start(mFilePath);
    mMsgHandlerManager->Start();
}

void AlprClientApp::onStop()
{
    mFrameGenerator->Stop();
    mMsgHandlerManager->Stop();

    QApplication::quit();
}

void AlprClientApp::onPause()
{
    mFrameGenerator->Pause();
    mMsgHandlerManager->Stop();
}

void AlprClientApp::onResume()
{
    mFrameGenerator->Resume();
    mMsgHandlerManager->Start();
}

void AlprClientApp::closeEvent(QCloseEvent* event)
{
    qDebug() << "call closeEvent tid:" << QThread::currentThreadId();

    mFrameGenerator->Stop();
    mMsgHandlerManager->Stop();
}

AlprClientApp::~AlprClientApp()
{
    delete ui;
}
