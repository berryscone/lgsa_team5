#include "LoginWindow.h"
#include "ui_LoginWindow.h"

#include <QTime>
#include <QDebug>

#define LOGIN_SKIP 0

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ALPR Client App");

    //connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWindow::onLogin); //TODO : 서버 테스트 환경 준비되면 enable
    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginDemo);

    manager = new QNetworkAccessManager(this);
    ui->lineEdit_ip->setText("https://localhost");
    ui->lineEdit_pw->setEchoMode(QLineEdit::Password);
    ui->lineEdit_id->setText("team5"); //dummy
    ui->lineEdit_pw->setText("1234"); //dummy
}

LoginWindow::~LoginWindow()
{
    delete ui;
    delete manager;
}

void LoginWindow::onLoginDemo()
{
#if LOGIN_SKIP
    launchProgram();
#else
    if (ui->lineEdit_id->text() == "team5" && ui->lineEdit_pw->text() == "1234") {
        ui->label_result->setText("success!");
        launchProgram();
    } else {
        ui->label_result->setText("Login Result : failed!");
        ui->lineEdit_id->clear();
        ui->lineEdit_pw->clear();
    }
#endif
}

void LoginWindow::launchProgram()
{
    AlprClientApp* laptopAppWindow = new AlprClientApp();
    this->hide();
    laptopAppWindow->show();
}

QUrl LoginWindow::getUrl()
{
    QString url_str = ui->lineEdit_ip->text();
    QUrl url(ui->lineEdit_ip->text());
    return url;
}

void LoginWindow::onLogin()
{
    QUrl login_url(getUrl());
    login_url.setPath("/login/");

    auto id = ui->lineEdit_id->text();
    auto pw = ui->lineEdit_pw->text();
    QString auth = id + ":" + pw;
    QByteArray auth_base64 = auth.toLocal8Bit().toBase64();
    QString basic_auth = "Basic " + auth_base64;

    QNetworkRequest request(login_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", basic_auth.toLocal8Bit());

    QByteArray data;
    reply.reset(manager->post(request, data));
    connect(reply.get(), &QNetworkReply::finished, this, &LoginWindow::onLoginFinished);
    connect(reply.get(), &QNetworkReply::sslErrors, this, &LoginWindow::onSslErrors);
    connect(reply.get(), &QIODevice::readyRead, this, &LoginWindow::onLoginReadReady);
}

void LoginWindow::onLoginReadReady()
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    token = obj["token"].toString();
    //ui.lineedit_token->setText(token);
}

void LoginWindow::onLoginFinished()
{
    launchProgram();
}

void LoginWindow::onSslErrors(const QList<QSslError> &errors)
{

}
