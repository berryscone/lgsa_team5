#include "LoginWindow.h"
#include "ui_LoginWindow.h"

#include <QTime>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    mNetworkManager = std::make_unique<NetworkManager>();

    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWindow::OnLogin);

    connect(this, SIGNAL(signalStartNetworkManager()),
                mNetworkManager.get(), SLOT(OnStart()));
    connect(this, SIGNAL(signalStopNetworkManager()),
                mNetworkManager.get(), SLOT(OnStop()));
    connect(this, SIGNAL(signalRequestLogin(QString,QString,QString)),
                mNetworkManager.get(), SLOT(OnRequestLogin(QString,QString,QString)));
    connect(mNetworkManager.get(), SIGNAL(ResponseLoginResult(bool)),
                this, SLOT(OnResponseLoginResult(bool)));

    ui->lineEdit_ip->setText("https://peaceful-atoll-24696.herokuapp.com");
    ui->lineEdit_pw->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::OnLogin()
{
    emit signalStartNetworkManager();
    RequestLogin(ui->lineEdit_ip->text(), ui->lineEdit_id->text(), ui->lineEdit_pw->text());
}

void LoginWindow::LaunchProgram()
{
    AlprClientApp* alprClientAppWindow = new AlprClientApp();
    this->hide();
    alprClientAppWindow->show();
}

void LoginWindow::RequestLogin(QString url, QString id, QString pw)
{
    emit signalRequestLogin(url, id, pw);
}

void LoginWindow::OnResponseLoginResult(bool isLoginSuccess)
{
    emit signalStopNetworkManager();

    if (isLoginSuccess) {
        ui->label_result->setText("Login Result : success!");
        LaunchProgram();
    } else {
        ui->label_result->setText("Login Result : failed!");
        ui->lineEdit_id->clear();
        ui->lineEdit_pw->clear();
    }
}
