#include "LoginWindow.h"
#include "ui_LoginWindow.h"

#include <QTime>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    mLoginProvider(NetworkManager::GetInstance()),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->lineEdit_pw->setEchoMode(QLineEdit::Password);

    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWindow::OnLogin);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::OnLogin()
{
    mLoginProvider.RequestLogin(
        ui->lineEdit_id->text(),
        ui->lineEdit_pw->text(),
        std::bind(&LoginWindow::OnLoginFinished, this, std::placeholders::_1, std::placeholders::_2));

    // TODO: UI �� �𷡽ð� ó�� ��׶��� ������ �˸� �� �ִ� ǥ�� �߰�
}

void LoginWindow::OnLoginFinished(const bool success, const QString detail)
{
    // TODO: OnLogin ���� ���۽�Ų ��׶��� ���� UI �� ����

    if (success) {
        ui->label_result->setText("Success");
        LaunchProgram();
    }
    else {
        ui->label_result->setText(detail);
        ui->lineEdit_id->clear();
        ui->lineEdit_pw->clear();
    }
}

void LoginWindow::LaunchProgram()
{
    AlprClientApp* alprClientAppWindow = new AlprClientApp();
    this->hide();
    alprClientAppWindow->show();
}
