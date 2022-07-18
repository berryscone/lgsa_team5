#include "AlprClientApp.h"
#include "handler/VehicleDetailHandler.h"

#include "RecentPlateCustomWidget.h"

#include <QMessageBox>

#define USE_IMAGE_BUTTON

AlprClientApp::AlprClientApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlprClientAppClass())
    , mbIsStart(false)
    , mLicensePlateImageWidth(140)
    , mLicensePlateImageHeight(50)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    ui->setupUi(this);
    this->setWindowTitle("ALPR Client App");

    ui->playbackView->setScene(new QGraphicsScene(this));
    ui->playbackView->scene()->addItem(&mPlaybackPixmap);

    ui->vehicleInfo->setWordWrap(true);
    ui->alertInfo->setWordWrap(true);

    // Connect Video Control Widgets
    connect(ui->openButton, &QPushButton::clicked, this, &AlprClientApp::OnOpen);
    connect(ui->stopButton, &QPushButton::clicked, this, &AlprClientApp::OnStop);
    connect(ui->toggleButton, &QPushButton::toggled, this, &AlprClientApp::OnToggle);

    mMsgHandlerManager = std::make_unique<MsgHandlerManager>();
    mDebugInfoMsgHandler = std::make_unique<DebugInfoMsgHandler>();

    mMsgHandlerManager->AddMsgHandler(mDebugInfoMsgHandler.get());

    connect(ui->recentPlatesListView, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(OnRecentPlatesViewItemClicked(QListWidgetItem*)));

    connect(mDebugInfoMsgHandler.get(), SIGNAL(UpdateLaptopAppUi(QString)),
            this, SLOT(UpdateDebugInfoView(QString)));

    connect(&VehicleDetailHandler::GetInstance(), &VehicleDetailHandler::SignalVehicleDetailPublish,
            this, &AlprClientApp::UpdateUI);

    connect(&NetworkManager::GetInstance(), &NetworkManager::SignalNetworkStatusChanged,
            this, &AlprClientApp::UpdateNetworkStatusUI);

    InitFrameGenerator();

#ifdef USE_IMAGE_BUTTON
    SetFrameGeneratorButtonStyle();
#endif
}

void AlprClientApp::InitFrameGenerator()
{
    connect(&mFrameGenerator, &FrameGenerator::UpdateLaptopAppUi, this, &AlprClientApp::UpdatePlaybackView);
    connect(&mFrameGenerator, &FrameGenerator::SignalVideoStopped, this, &AlprClientApp::OnVideoStopped);

    connect(this, &AlprClientApp::startFrameGenerator, &mFrameGenerator, &FrameGenerator::Start);
    connect(this, &AlprClientApp::pauseFrameGenerator, &mFrameGenerator, &FrameGenerator::Pause);
    connect(this, &AlprClientApp::resumeFrameGenerator, &mFrameGenerator, &FrameGenerator::Resume);
    connect(this, &AlprClientApp::stopFrameGenerator, &mFrameGenerator, &FrameGenerator::Stop);

    mFrameGenerator.moveToThread(&mFrameGeneratorThread);
    mFrameGeneratorThread.start();
}

void AlprClientApp::UpdateUI(const QImage plate_image, const QJsonObject vehicle_detail)
{
    QJsonArray vehicleDetailArray = vehicle_detail["vehicle_details"].toArray();
    QImage resizePlateImage = plate_image;
    resizePlateImage = resizePlateImage.scaled(mLicensePlateImageWidth, mLicensePlateImageHeight);

    if (vehicleDetailArray.size() > 0) {
        QJsonObject vehicleDetailJsonObject = vehicleDetailArray.at(0).toObject();

        //UpdateRecentPltesView
        QString licensePlatesNumber = vehicleDetailJsonObject["plate_number"].toString();
        UpdateRecentPlatesView(resizePlateImage, vehicleDetailJsonObject);

        //UpdateVehicleInfoView
        UpdateVehicleInfoView(resizePlateImage, vehicleDetailJsonObject);

        //UpdateAlertInfoView
        UpdateAlertInfoView(resizePlateImage, vehicleDetailJsonObject);
    }
}

void AlprClientApp::UpdatePlaybackView(QPixmap pixmap)
{   
    mPlaybackPixmap.setPixmap(pixmap);
    ui->playbackView->fitInView(&mPlaybackPixmap, Qt::KeepAspectRatio);

    qApp->processEvents();
}

void AlprClientApp::UpdateRecentPlatesView(QImage &licensePlateImage, QJsonObject &vehicleDetailJsonObject)
{
    QString vehicleDetailInfoStr;

    vehicleDetailInfoStr.append("Number : " + vehicleDetailJsonObject["plate_number"].toString() + "\n");
    vehicleDetailInfoStr.append("Make : " + vehicleDetailJsonObject["make"].toString() + "\n");
    vehicleDetailInfoStr.append("Model : " + vehicleDetailJsonObject["model"].toString() + "\n");
    vehicleDetailInfoStr.append("Color : " + vehicleDetailJsonObject["color"].toString());

    RecentPlateCustomWidget *wigetItem = new RecentPlateCustomWidget(licensePlateImage, vehicleDetailInfoStr);
    QListWidgetItem* plate = new QListWidgetItem;
    plate->setSizeHint(QSize(mLicensePlateImageWidth, mLicensePlateImageHeight));
    plate->setText(vehicleDetailJsonObject["plate_number"].toString());
    plate->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    plate->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->recentPlatesListView->addItem(plate);
    ui->recentPlatesListView->setItemWidget(plate, wigetItem);
    ui->recentPlatesListView->setSpacing(3);
    ui->recentPlatesListView->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    ui->recentPlatesListView->scrollToBottom();
    ui->recentPlatesListView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->recentPlatesListView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    ui->recentPlatesListView->setAlternatingRowColors(true);
}

void AlprClientApp::UpdateVehicleInfoView(QImage &licensePlateImage, QJsonObject &vehicleDetailJsonObject)
{
    QString vehicleDetailInfoStr;

    vehicleDetailInfoStr.append("Number : " + vehicleDetailJsonObject["plate_number"].toString() + "\n");
    vehicleDetailInfoStr.append("Make : " + vehicleDetailJsonObject["make"].toString() + "\n");
    vehicleDetailInfoStr.append("Model : " + vehicleDetailJsonObject["model"].toString() + "\n");
    vehicleDetailInfoStr.append("Color : " + vehicleDetailJsonObject["color"].toString());

    ui->lastLicensePlateView->setPixmap(QPixmap(QPixmap::fromImage(licensePlateImage)));
    ui->vehicleInfo->setText(vehicleDetailInfoStr);
    ui->vehicleInfo->setWordWrap(true);
}

void AlprClientApp::UpdateDebugInfoView(QString debugInfo)
{
    ui->debugInfo->setText(debugInfo);
}

void AlprClientApp::UpdateAlertInfoView(QImage &licensePlateImage, QJsonObject &vehicleDetailJsonObject)
{
    QString alertVehicleDetailInfoStr;

    if (vehicleDetailJsonObject["status"].toString() != "No Wants / Warrants") {
        alertVehicleDetailInfoStr.append("Number : " + vehicleDetailJsonObject["plate_number"].toString() + "\n");
        alertVehicleDetailInfoStr.append("Reason : " + vehicleDetailJsonObject["status"].toString() + "\n");
        alertVehicleDetailInfoStr.append("Owner : " + vehicleDetailJsonObject["owner"].toString() + "\n");
        alertVehicleDetailInfoStr.append("Address : " + vehicleDetailJsonObject["address"].toString() + "\n");
        alertVehicleDetailInfoStr.append("Make : " + vehicleDetailJsonObject["make"].toString() + "\n");
        alertVehicleDetailInfoStr.append("Model : " + vehicleDetailJsonObject["model"].toString() + "\n");
        alertVehicleDetailInfoStr.append("Color : " + vehicleDetailJsonObject["color"].toString());

        ui->alertLicensePlateView->setPixmap(QPixmap(QPixmap::fromImage(licensePlateImage)));
        ui->alertInfo->setText(alertVehicleDetailInfoStr);
    }
}

void AlprClientApp::OnOpen()
{
    QFileDialog fd;
#if 0
    fd.show();
    fd.exec();
    QString strFileName = fd.selectedFiles().at(0);
    mFrameGenerator.SetOpenFilePath(strFileName);
#else   // 다이얼로그 취소한 경우 예외처리
    QString strFileName = fd.getOpenFileName(0, tr("Open Video File..."), "", "");

    if (strFileName.isEmpty())  
        return;

    mFrameGenerator.SetOpenFilePath(strFileName);
#endif    

    qDebug() << "call onOpen tid:" << QThread::currentThreadId() << ", mFilePath:" << mFilePath.data();

    emit startFrameGenerator();
    mMsgHandlerManager->Start();

    mbIsStart = true;

    SetControllerRun();
}

void AlprClientApp::OnStop()
{
    emit stopFrameGenerator();
    SetControllerStop();
}

void AlprClientApp::OnToggle(bool bIsPause)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    if (ui->toggleButton->isEnabled())  // open -> pause -> stop -> open -> pause 할 때 정상적으로 toggle 동작되지 않는 현상 수정
    {
        if (bIsPause) {
            emit pauseFrameGenerator();
            mMsgHandlerManager->Stop();
            SetToggleButtonToPlay();

        }
        else {
            emit resumeFrameGenerator();
            mMsgHandlerManager->Start();
            SetToggleButtonToPause();
        }
    }
}

void AlprClientApp::OnVideoStopped()
{
    SetControllerStop();
}

void AlprClientApp::SetControllerRun()
{
    ui->openButton->setEnabled(false);
    ui->toggleButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    SetToggleButtonToPause();
}

void AlprClientApp::SetControllerStop()
{
    ui->openButton->setEnabled(true);
    ui->toggleButton->setEnabled(false);
    if (ui->toggleButton->isChecked())  // open -> pause -> stop -> open -> pause 할 때 정상적으로 toggle 동작되지 않는 현상 수정
        ui->toggleButton->toggle();
    ui->stopButton->setEnabled(false);
    SetToggleButtonToPlay();
}

void AlprClientApp::SetToggleButtonToPause()
{
#ifdef USE_IMAGE_BUTTON
    ui->toggleButton->setStyleSheet(
        "                                                               \
        QPushButton {                                                   \
            border-image: url(:/assets/images/pauseButton.png);         \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:disabled {                                          \
            border-image: url(:/assets/images/pauseButtonDisabled.png); \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:hover {                                             \
            border-image: url(:/assets/images/pauseButtonHover.png);    \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:pressed {                                           \
            border-image: url(:/assets/images/pauseButtonPressed.png);  \
            background-repeat: no-repeat;                               \
        }                                                               \
        ");
#else
    ui->toggleButton->setText("Pause");
#endif
}

void AlprClientApp::SetToggleButtonToPlay()
{
#ifdef USE_IMAGE_BUTTON
    ui->toggleButton->setStyleSheet(
        "                                                               \
        QPushButton {                                                   \
            border-image: url(:/assets/images/playButton.png);          \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:disabled {                                          \
            border-image: url(:/assets/images/playButtonDisabled.png);  \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:hover {                                             \
            border-image: url(:/assets/images/playButtonHover.png);     \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:pressed {                                           \
            border-image: url(:/assets/images/playButtonPressed.png);   \
            background-repeat: no-repeat;                               \
        }                                                               \
        ");
#else
    ui->toggleButton->setText("Play");
#endif
}

void AlprClientApp::SetFrameGeneratorButtonStyle()
{
    int imageButtonWidth = 24;
    int imageButtonHeight = 24;

    ui->openButton->setStyleSheet(
        "                                                               \
        QPushButton {                                                   \
            border-image: url(:/assets/images/openButton.png);          \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:disabled {                                          \
            border-image: url(:/assets/images/openButtonDisabled.png);  \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:hover {                                             \
            border-image: url(:/assets/images/openButtonHover.png);     \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:pressed {                                           \
            border-image: url(:/assets/images/openButtonPressed.png);   \
            background-repeat: no-repeat;                               \
        }                                                               \
        ");

    ui->openButton->setFixedWidth(imageButtonWidth);
    ui->openButton->setFixedHeight(imageButtonHeight);
    ui->openButton->setText("");

    ui->toggleButton->setStyleSheet(
        "                                                               \
        QPushButton {                                                   \
            border-image: url(:/assets/images/playButton.png);          \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:disabled {                                          \
            border-image: url(:/assets/images/playButtonDisabled.png);  \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:hover {                                             \
            border-image: url(:/assets/images/playButtonHover.png);     \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:pressed {                                           \
            border-image: url(:/assets/images/playButtonPressed.png);   \
            background-repeat: no-repeat;                               \
        }                                                               \
        ");

    ui->toggleButton->setFixedWidth(imageButtonWidth);
    ui->toggleButton->setFixedHeight(imageButtonHeight);
    ui->toggleButton->setText("");

    ui->stopButton->setStyleSheet(
        "                                                               \
        QPushButton {                                                   \
            border-image: url(:/assets/images/stopButton.png);          \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:disabled {                                          \
            border-image: url(:/assets/images/stopButtonDisabled.png);  \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:hover {                                             \
            border-image: url(:/assets/images/stopButtonHover.png);     \
            background-repeat: no-repeat;                               \
        }                                                               \
        QPushButton:pressed {                                           \
            border-image: url(:/assets/images/stopButtonPressed.png);   \
            background-repeat: no-repeat;                               \
        }                                                               \
        ");

    ui->stopButton->setFixedWidth(imageButtonWidth);
    ui->stopButton->setFixedHeight(imageButtonHeight);
    ui->stopButton->setText("");

    ui->horizontalLayout->addWidget(ui->openButton);
    ui->horizontalLayout->addWidget(ui->toggleButton);
    ui->horizontalLayout->addWidget(ui->stopButton);
    ui->horizontalLayout->setAlignment(Qt::AlignLeft);
}

void AlprClientApp::OnRecentPlatesViewItemClicked(QListWidgetItem *item)
{
    RecentPlateCustomWidget *widgetItem = dynamic_cast<RecentPlateCustomWidget*>(ui->recentPlatesListView->itemWidget(item));
    QMessageBox msgBox;

    msgBox.setIconPixmap(widgetItem->GetLicensePlatePixmap());
    msgBox.setWindowTitle("Detail Vehicle Information");
    msgBox.setText(widgetItem->GetVehicleDetailInfo());
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void AlprClientApp::UpdateNetworkStatusUI(QNetworkReply::NetworkError status)
{
    qDebug() << __FUNCTION__ << " NetworkError : " << status;
    unique_ptr<QPixmap> statusIconPixmap;

    if (status == QNetworkReply::NetworkError::NoError) {
        statusIconPixmap = make_unique<QPixmap>(QDir::currentPath()+"/assets/images/greenButton.png");
    } else {
        statusIconPixmap = make_unique<QPixmap>(QDir::currentPath()+"/assets/images/redButton.png");
    }

    ui->networkStatusIcon->setPixmap(*statusIconPixmap);
}

void AlprClientApp::closeEvent(QCloseEvent* event)
{
    qDebug() << "call closeEvent tid:" << QThread::currentThreadId();

    emit stopFrameGenerator();
    mMsgHandlerManager->Stop();

    QApplication::quit();
}

AlprClientApp::~AlprClientApp()
{
    delete ui;
}
