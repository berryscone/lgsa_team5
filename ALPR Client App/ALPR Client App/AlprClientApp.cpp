#include "AlprClientApp.h"
#include "handler/VehicleDetailHandler.h"

#include "RecentPlateCustomWidget.h"

#include <QMessageBox>

// #define USE_IMAGE_BUTTON

AlprClientApp::AlprClientApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlprClientAppClass())
    , mLicensePlateImageWidth(140)
    , mLicensePlateImageHeight(50)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    ui->setupUi(this);
    this->setWindowTitle("ALPR Client App");

    mIconNetIndicatorRed = QPixmap(":/assets/images/redButton.png");
    mIconNetIndicatorGreen = QPixmap(":/assets/images/greenButton.png");
    ui->label_net_status_indicator->setPixmap(mIconNetIndicatorGreen);

    ui->graphics_playback->setScene(new QGraphicsScene(this));
    ui->graphics_playback->scene()->addItem(&mPlaybackPixmap);

    ui->label_alert_info->setWordWrap(true);
    ui->label_vehicle_info->setWordWrap(true);

    // Connect Video Control Widgets
    connect(ui->push_open, &QPushButton::clicked, this, &AlprClientApp::OnOpen);
    connect(ui->push_stop, &QPushButton::clicked, this, &AlprClientApp::OnStop);
    connect(ui->push_pause, &QPushButton::toggled, this, &AlprClientApp::OnToggle);

    mMsgHandlerManager = std::make_unique<MsgHandlerManager>();
    mDebugInfoMsgHandler = std::make_unique<DebugInfoMsgHandler>();

    mMsgHandlerManager->AddMsgHandler(mDebugInfoMsgHandler.get());

    connect(ui->list_recent_plates, SIGNAL(itemClicked(QListWidgetItem*)),
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
    ui->graphics_playback->fitInView(&mPlaybackPixmap, Qt::KeepAspectRatio);

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

    ui->list_recent_plates->addItem(plate);
    ui->list_recent_plates->setItemWidget(plate, wigetItem);
    ui->list_recent_plates->setSpacing(3);
    ui->list_recent_plates->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    ui->list_recent_plates->scrollToBottom();
    ui->list_recent_plates->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->list_recent_plates->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    ui->list_recent_plates->setAlternatingRowColors(true);
}

void AlprClientApp::UpdateVehicleInfoView(QImage &licensePlateImage, QJsonObject &vehicleDetailJsonObject)
{
    QString vehicleDetailInfoStr;

    vehicleDetailInfoStr.append("Number : " + vehicleDetailJsonObject["plate_number"].toString() + "\n");
    vehicleDetailInfoStr.append("Make : " + vehicleDetailJsonObject["make"].toString() + "\n");
    vehicleDetailInfoStr.append("Model : " + vehicleDetailJsonObject["model"].toString() + "\n");
    vehicleDetailInfoStr.append("Color : " + vehicleDetailJsonObject["color"].toString());

    ui->label_last_plate->setPixmap(QPixmap(QPixmap::fromImage(licensePlateImage)));
    ui->label_vehicle_info->setText(vehicleDetailInfoStr);
    ui->label_vehicle_info->setWordWrap(true);
}

void AlprClientApp::UpdateDebugInfoView(QString debugInfo)
{
    ui->label_debug_info->setText(debugInfo);
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

        ui->label_alert_plate->setPixmap(QPixmap(QPixmap::fromImage(licensePlateImage)));
        ui->label_alert_info->setText(alertVehicleDetailInfoStr);
    }
}

void AlprClientApp::OnOpen()
{
    QFileDialog fd;
    QString strFileName = fd.getOpenFileName(0, tr("Open Video File..."), "", "");
    if (strFileName.isEmpty()) {
        qDebug() << "File Open Canceled";
        return;
    }
    mFrameGenerator.SetOpenFilePath(strFileName);

    qDebug() << "call onOpen tid:" << QThread::currentThreadId() << ", mFilePath:" << mFilePath.data();

    emit startFrameGenerator();
    mMsgHandlerManager->Start();

    SetControllerRun();
}

void AlprClientApp::OnStop()
{
    emit stopFrameGenerator();
    SetControllerStop();
}

void AlprClientApp::OnToggle(bool checked)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    if (checked) {
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

void AlprClientApp::OnVideoStopped()
{
    SetControllerStop();
}

void AlprClientApp::SetControllerRun()
{
    ui->push_open->setEnabled(false);
    ui->push_pause->setEnabled(true);
    ui->push_stop->setEnabled(true);
    SetToggleButtonToPause();
}

void AlprClientApp::SetControllerStop()
{
    ui->push_open->setEnabled(true);
    ui->push_pause->setEnabled(false);
    ui->push_stop->setEnabled(false);
    SetToggleButtonToPlay();
}

void AlprClientApp::SetToggleButtonToPause()
{
#ifdef USE_IMAGE_BUTTON
    ui->push_pause->setStyleSheet(
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
    ui->push_pause->setText("Pause");
    ui->push_pause->clicked(true);
#endif
}

void AlprClientApp::SetToggleButtonToPlay()
{
#ifdef USE_IMAGE_BUTTON
    ui->push_pause->setStyleSheet(
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
    ui->push_pause->setText("Play");
    ui->push_pause->clicked(false);
}
#endif

void AlprClientApp::SetFrameGeneratorButtonStyle()
{
    int imageButtonWidth = 24;
    int imageButtonHeight = 24;

    ui->push_open->setStyleSheet(
        "                                                               \
        QPushButton {                                                   \
            border-image: url(:/assets/images/push_open.png);          \
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

    ui->push_open->setFixedWidth(imageButtonWidth);
    ui->push_open->setFixedHeight(imageButtonHeight);
    ui->push_open->setText("");

    ui->push_pause->setStyleSheet(
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

    ui->push_pause->setFixedWidth(imageButtonWidth);
    ui->push_pause->setFixedHeight(imageButtonHeight);
    ui->push_pause->setText("");

    ui->push_stop->setStyleSheet(
        "                                                               \
        QPushButton {                                                   \
            border-image: url(:/assets/images/push_stop.png);          \
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

    ui->push_stop->setFixedWidth(imageButtonWidth);
    ui->push_stop->setFixedHeight(imageButtonHeight);
    ui->push_stop->setText("");

    ui->horizontalLayout->addWidget(ui->push_open);
    ui->horizontalLayout->addWidget(ui->push_pause);
    ui->horizontalLayout->addWidget(ui->push_stop);
    ui->horizontalLayout->setAlignment(Qt::AlignLeft);
}

void AlprClientApp::OnRecentPlatesViewItemClicked(QListWidgetItem *item)
{
    RecentPlateCustomWidget *widgetItem = dynamic_cast<RecentPlateCustomWidget*>(ui->list_recent_plates->itemWidget(item));
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

    if (status == QNetworkReply::NetworkError::NoError) {
        ui->label_net_status_indicator->setPixmap(mIconNetIndicatorGreen);
    } else {
        ui->label_net_status_indicator->setPixmap(mIconNetIndicatorRed);
    }
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
