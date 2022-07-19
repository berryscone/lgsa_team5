#include "MainWindow.h"

#include <QMessageBox>

#include "View/RecentPlateWidget.h"
#include "Controller/Network/NetworkManager.h"

// #define USE_IMAGE_BUTTON

MainWindow::MainWindow(QWidget *parent)
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

    // Connect Video Control Widgets
    connect(ui->push_open, &QPushButton::clicked, this, &MainWindow::OnOpen);
    connect(ui->push_stop, &QPushButton::clicked, this, &MainWindow::OnStop);
    connect(ui->push_pause, &QPushButton::toggled, this, &MainWindow::OnToggle);

    connect(ui->list_recent_plates, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(OnRecentPlatesViewItemClicked(QListWidgetItem*)));

    connect(&VehicleDetailHandler::GetInstance(), &VehicleDetailHandler::SignalVehicleDetailPublish,
            this, &MainWindow::UpdateUI);

    connect(&NetworkManager::GetInstance(), &NetworkManager::SignalNetworkStatusChanged,
            this, &MainWindow::UpdateNetworkStatusUI);

    InitFrameGenerator();

#ifdef USE_IMAGE_BUTTON
    SetFrameGeneratorButtonStyle();
#endif
}

void MainWindow::InitFrameGenerator()
{
    connect(&mFrameGenerator, &FrameGenerator::UpdateLaptopAppUi, this, &MainWindow::UpdatePlaybackView);
    connect(&mFrameGenerator, &FrameGenerator::SignalVideoStopped, this, &MainWindow::OnVideoStopped);
    connect(&mFrameGenerator, &FrameGenerator::SignalUpdateDebugInfo, this, &MainWindow::UpdateDebugInfoView);

    connect(this, &MainWindow::startFrameGenerator, &mFrameGenerator, &FrameGenerator::Start);
    connect(this, &MainWindow::pauseFrameGenerator, &mFrameGenerator, &FrameGenerator::Pause);
    connect(this, &MainWindow::resumeFrameGenerator, &mFrameGenerator, &FrameGenerator::Resume);
    connect(this, &MainWindow::stopFrameGenerator, &mFrameGenerator, &FrameGenerator::Stop);

    mFrameGenerator.moveToThread(&mFrameGeneratorThread);
    mFrameGeneratorThread.start();
}

void MainWindow::UpdateUI(const VehicleDetail vehicleDetail)
{
    if (vehicleDetail.status == VehicleDetail::Normal) {
        UpdateRecentPlatesView(vehicleDetail);
        UpdateVehicleInfoView(vehicleDetail);
    }
    else {
        UpdateRecentPlatesView(vehicleDetail);
        UpdateAlertInfoView(vehicleDetail);
    }
}

void MainWindow::UpdatePlaybackView(QPixmap pixmap)
{   
    mPlaybackPixmap.setPixmap(pixmap);
    ui->graphics_playback->fitInView(&mPlaybackPixmap, Qt::KeepAspectRatio);

    qApp->processEvents();
}

void MainWindow::UpdateRecentPlatesView(const VehicleDetail& vehicleDetail)
{
    RecentPlateWidget* wigetItem = new RecentPlateWidget(vehicleDetail);
    QListWidgetItem* plate = new QListWidgetItem;
    plate->setSizeHint(QSize(mLicensePlateImageWidth, mLicensePlateImageHeight));
    plate->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->list_recent_plates->addItem(plate);
    ui->list_recent_plates->setItemWidget(plate, wigetItem);
    ui->list_recent_plates->scrollToBottom();
}

void MainWindow::UpdateVehicleInfoView(const VehicleDetail& vehicleDetail)
{
    ui->line_info_number->setText(vehicleDetail.number);
    ui->line_info_make->setText(vehicleDetail.make);
    ui->line_info_model->setText(vehicleDetail.model);
    ui->line_info_color->setText(vehicleDetail.color);
    ui->label_last_plate->setPixmap(QPixmap::fromImage(vehicleDetail.image));
}

void MainWindow::UpdateDebugInfoView(QString debugInfo)
{
    ui->label_debug_info->setText(debugInfo);
}

void MainWindow::UpdateAlertInfoView(const VehicleDetail& vehicleDetail)
{
    ui->label_alert_plate->setPixmap(QPixmap::fromImage(vehicleDetail.image));
    ui->line_alert_number->setText(vehicleDetail.number);
    ui->line_alert_reason->setText(vehicleDetail.rawStatus);
    ui->line_alert_make->setText(vehicleDetail.make);
    ui->line_alert_model->setText(vehicleDetail.model);
    ui->line_alert_color->setText(vehicleDetail.color);
    ui->line_alert_owner->setText(vehicleDetail.owner);
    ui->text_alert_address->setText(vehicleDetail.address);
}

void MainWindow::OnOpen()
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
    SetControllerRun();
}

void MainWindow::OnStop()
{
    emit stopFrameGenerator();
    SetControllerStop();
}

void MainWindow::OnToggle(bool checked)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    if (checked) {
        emit pauseFrameGenerator();
        SetToggleButtonToPlay();

    }
    else {
        emit resumeFrameGenerator();
        SetToggleButtonToPause();
    }
}

void MainWindow::OnVideoStopped()
{
    SetControllerStop();
}

void MainWindow::SetControllerRun()
{
    ui->push_open->setEnabled(false);
    ui->push_pause->setEnabled(true);
    ui->push_stop->setEnabled(true);
    SetToggleButtonToPause();
}

void MainWindow::SetControllerStop()
{
    ui->push_open->setEnabled(true);
    ui->push_pause->setEnabled(false);
    ui->push_stop->setEnabled(false);
    SetToggleButtonToPlay();
}

void MainWindow::SetToggleButtonToPause()
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

void MainWindow::SetToggleButtonToPlay()
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

void MainWindow::SetFrameGeneratorButtonStyle()
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

    ui->hbox_control->addWidget(ui->push_open);
    ui->hbox_control->addWidget(ui->push_pause);
    ui->hbox_control->addWidget(ui->push_stop);
    ui->hbox_control->setAlignment(Qt::AlignLeft);
}

void MainWindow::OnRecentPlatesViewItemClicked(QListWidgetItem *item)
{
    RecentPlateWidget* widgetItem = dynamic_cast<RecentPlateWidget*>(ui->list_recent_plates->itemWidget(item));
    QMessageBox msgBox;

    msgBox.setIconPixmap(widgetItem->GetLicensePlatePixmap());
    msgBox.setWindowTitle("Detail Vehicle Information");
    msgBox.setText(widgetItem->GetVehicleDetailInfo());
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void MainWindow::UpdateNetworkStatusUI(QNetworkReply::NetworkError status)
{
    qDebug() << __FUNCTION__ << " NetworkError : " << status;

    if (status == QNetworkReply::NetworkError::NoError) {
        ui->label_net_status_indicator->setPixmap(mIconNetIndicatorGreen);
    } else {
        ui->label_net_status_indicator->setPixmap(mIconNetIndicatorRed);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "call closeEvent tid:" << QThread::currentThreadId();

    emit stopFrameGenerator();
    QApplication::quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}
