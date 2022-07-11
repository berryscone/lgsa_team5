#include "LaptopApp.h"
#include "ui_LaptopApp.h"

LaptopApp::LaptopApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LaptopApp)
    , mbIsStart(false)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    ui->setupUi(this);

    ui->playbackView->setScene(new QGraphicsScene(this));
    ui->playbackView->scene()->addItem(&mPlaybackPixmap);
    ui->vehicleInfo->setWordWrap(true);

    connect(ui->openButton, &QPushButton::clicked, this, &LaptopApp::onOpen);
    connect(ui->toggleButton, SIGNAL(toggled(bool)), this, SLOT(onToggle(bool)));

    mMsgHandlerManager = std::make_unique<MsgHandlerManager>();
    mFrameMsgHandler = std::make_unique<FrameMsgHandler>();
    mRecentPlatesMsgHandler = std::make_unique<RecentPlatesMsgHandler>();
    mVehicleInfoMsgHandler = std::make_unique<VehicleInfoMsgHandler>();
    mDebugInfoMsgHandler = std::make_unique<DebugInfoMsgHandler>();

    mMsgHandlerManager->AddMsgHandler(mFrameMsgHandler.get());
    mMsgHandlerManager->AddMsgHandler(mRecentPlatesMsgHandler.get());
    mMsgHandlerManager->AddMsgHandler(mVehicleInfoMsgHandler.get());
    mMsgHandlerManager->AddMsgHandler(mDebugInfoMsgHandler.get());

    connect(mRecentPlatesMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QImage)),
            this, SLOT(UpdateRecentPlatesView(QImage)));

    connect(mVehicleInfoMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QString)),
            this, SLOT(UpdateVehicleInfoView(QString)));

    connect(mDebugInfoMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QString)),
            this, SLOT(UpdateDebugInfoView(QString)));

//    TODO : frameModel, frameMsgHandler 사용하지 않을 경우 추후 삭제
//    connect(mFrameMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QImage)),
//            this, SLOT(UpdatePlaybackView(QImage)));

    //NOTE : frameGenerator runs in a separate thread
    makeFrameGeneratorThread();
}

void LaptopApp::makeFrameGeneratorThread()
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    mFrameGeneratorThread = new QThread();
    mFrameGenerator = std::make_unique<FrameGenerator>();

    connect(mFrameGenerator.get(), SIGNAL(UpdateLaptopAppUi(QPixmap)),
            this, SLOT(UpdatePlaybackView(QPixmap)));

    connect(mFrameGeneratorThread, SIGNAL(started()), mFrameGenerator.get(), SLOT(Start()));
    connect(mFrameGeneratorThread, SIGNAL(finished()), mFrameGenerator.get(), SLOT(Stop()));

    connect(this, SIGNAL(startFrameGenerator()), mFrameGenerator.get(), SLOT(Start()));
    connect(this, SIGNAL(pauseFrameGenerator()), mFrameGenerator.get(), SLOT(Pause()));
    connect(this, SIGNAL(resumeFrameGenerator()), mFrameGenerator.get(), SLOT(Resume()));
    connect(this, SIGNAL(stopFrameGenerator()), mFrameGenerator.get(), SLOT(Stop()));

    mFrameGenerator->moveToThread(mFrameGeneratorThread);
}

void LaptopApp::UpdatePlaybackView(QPixmap pixmap)
{   
    mPlaybackPixmap.setPixmap(pixmap);
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
    //qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId() << ", vehicleInfo: " << info;

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
    mFrameGenerator->SetOpenFilePath(mFilePath);

    qDebug() << "call onOpen tid:" << QThread::currentThreadId() << ", mFilePath:" << mFilePath.data();

    mFrameGeneratorThread->start();
    emit startFrameGenerator();
    mMsgHandlerManager->Start();

    mbIsStart = true;

    //TODO : 사용자가 open 버튼을 다시 눌러서 다른 파일을 선택하려고 할 경우 동작 지원을 하려면 추가 수정 필요
    //       일단 UX로 해결 : 재생이 시작되면 open button을 비활성화
    ui->openButton->setEnabled(false);
}

void LaptopApp::onToggle(bool bIsPause)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    if (bIsPause) {
        emit pauseFrameGenerator();
        mMsgHandlerManager->Stop();
        ui->toggleButton->setText("Resume");

    } else {
        emit resumeFrameGenerator();
        mMsgHandlerManager->Start();
        ui->toggleButton->setText("Pause");
    }
}

void LaptopApp::closeEvent(QCloseEvent *event)
{
    qDebug() << "call closeEvent tid:" << QThread::currentThreadId();

    emit stopFrameGenerator();
    mMsgHandlerManager->Stop();

    QApplication::quit();
}

LaptopApp::~LaptopApp()
{
    delete ui;
}

