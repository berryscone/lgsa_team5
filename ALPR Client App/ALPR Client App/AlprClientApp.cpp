#include "AlprClientApp.h"
#include "handler/VehicleDetailHandler.h"

#include <QMessageBox>

AlprClientApp::AlprClientApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlprClientAppClass())
    , mbIsStart(false)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();
    ui->setupUi(this);
    this->setWindowTitle("ALPR Client App");

    ui->playbackView->setScene(new QGraphicsScene(this));
    ui->playbackView->scene()->addItem(&mPlaybackPixmap);

    ui->alertLicensePlateView->setScene(new QGraphicsScene(this));
    ui->alertLicensePlateView->scene()->addItem(&mAlertLicensePlatePixmap);

    ui->vehicleInfo->setWordWrap(true);
    ui->alertInfo->setWordWrap(true);

    connect(ui->openButton, &QPushButton::clicked, this, &AlprClientApp::onOpen);
    connect(ui->toggleButton, SIGNAL(toggled(bool)), this, SLOT(onToggle(bool)));

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

    //NOTE : frameGenerator runs in a separate thread
    makeFrameGeneratorThread();
}

void AlprClientApp::makeFrameGeneratorThread()
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

void AlprClientApp::UpdateUI(const QImage plate_image, const QJsonObject vehicle_detail)
{
    QJsonArray vehicleDetailArray = vehicle_detail["vehicle_details"].toArray();

//    qDebug() << "vehicle_details count:" << vehicle_detail["count"].toString();
//    qDebug() << "vehicle_details size:" << vehicleDetailArray.size();

    //UpdateRecentPltesView
    QString licensePlatesNumber = vehicle_detail["plate_number"].toString();
    UpdateRecentPlatesView(plate_image, licensePlatesNumber);

    //UpdateVehicleInfoView
    QString vehicleDetailInfo = licensePlatesNumber + " Detail Info!";
    UpdateVehicleInfoView(licensePlatesNumber);

    //UpdateAlertInfoView
    QString alertInfo = licensePlatesNumber + " Wanted!!!";
    UpdateAlertInfoView(plate_image, alertInfo);
}

void AlprClientApp::UpdatePlaybackView(QPixmap pixmap)
{   
    mPlaybackPixmap.setPixmap(pixmap);
    ui->playbackView->fitInView(&mPlaybackPixmap, Qt::KeepAspectRatio);

    qApp->processEvents();
}

void AlprClientApp::UpdateRecentPlatesView(QImage licensePlateImage, QString vehicleInfo)
{
    QLabel* label = new QLabel;
    int itemWidth = 140;
    int itemHeight = 50;

    licensePlateImage = licensePlateImage.scaled(itemWidth, itemHeight);
    label->setPixmap(QPixmap(QPixmap::fromImage(licensePlateImage)));
    QListWidgetItem* plate = new QListWidgetItem;
    plate->setSizeHint(QSize(itemWidth, itemHeight));
    plate->setText(vehicleInfo);
    plate->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    plate->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->recentPlatesListView->addItem(plate);
    ui->recentPlatesListView->setItemWidget(plate, label);
    ui->recentPlatesListView->setSpacing(3);
    ui->recentPlatesListView->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    ui->recentPlatesListView->scrollToBottom();
    ui->recentPlatesListView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->recentPlatesListView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
}

void AlprClientApp::UpdateVehicleInfoView(QString vehicleInfo)
{
    ui->vehicleInfo->setText(vehicleInfo);
    ui->vehicleInfo->setWordWrap(true);
}

void AlprClientApp::UpdateDebugInfoView(QString debugInfo)
{
    ui->debugInfo->setText(debugInfo);
}

void AlprClientApp::UpdateAlertInfoView(QImage licensePlateImage, QString alertInfo)
{
    mAlertLicensePlatePixmap.setPixmap(QPixmap(QPixmap::fromImage(licensePlateImage)));
    ui->alertLicensePlateView->fitInView(ui->alertLicensePlateView->scene()->sceneRect(), Qt::KeepAspectRatio);
    ui->alertInfo->setText(alertInfo);
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
    mFrameGenerator->SetOpenFilePath(mFilePath);

    qDebug() << "call onOpen tid:" << QThread::currentThreadId() << ", mFilePath:" << mFilePath.data();

    mFrameGeneratorThread->start();
    emit startFrameGenerator();
    mMsgHandlerManager->Start();

    mbIsStart = true;

    //TODO : ����ڰ� open ��ư�� �ٽ� ������ �ٸ� ������ �����Ϸ��� �� ��� ���� ������ �Ϸ��� �߰� ���� �ʿ�
    //       �ϴ� UX�� �ذ� : ����� ���۵Ǹ� open button�� ��Ȱ��ȭ
    ui->openButton->setEnabled(false);
    ui->toggleButton->setText("Pause");
}

void AlprClientApp::onToggle(bool bIsPause)
{
    qDebug() << "Function Name: " << Q_FUNC_INFO <<", tid:" << QThread::currentThreadId();

    if (bIsPause) {
        emit pauseFrameGenerator();
        mMsgHandlerManager->Stop();
        ui->toggleButton->setText("Play");

    } else {
        emit resumeFrameGenerator();
        mMsgHandlerManager->Start();
        ui->toggleButton->setText("Pause");
    }
}

void AlprClientApp::OnRecentPlatesViewItemClicked(QListWidgetItem *item)
{
    //TODO : item�� �����ϴ� label�� ���� �׷��� �� �ִ��� ���� �ʿ�
    //QWidget* itemWidget = ui->recentPlatesListView->itemWidget(item);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Detail Vehicle Information");
    msgBox.setText(item->text());
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
}

void AlprClientApp::UpdateNetworkStatusUI(QNetworkReply::NetworkError status)
{
    qDebug() << __FUNCTION__ << " NetworkError : " << status;
    QPixmap *statusIconPixmap;

    if (status == QNetworkReply::NetworkError::NoError) {
        statusIconPixmap = new QPixmap(QDir::currentPath()+"/assets/images/greenButton.png");
    } else {
        statusIconPixmap = new QPixmap(QDir::currentPath()+"/assets/images/redButton.png");
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
