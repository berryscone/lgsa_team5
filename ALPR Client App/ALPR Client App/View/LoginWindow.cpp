#include "LoginWindow.h"
#include "ui_LoginWindow.h"

#include <QTime>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    mNetworkManager(NetworkManager::GetInstance()),
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
    QString id = ui->lineEdit_id->text().trimmed();
    QString pw = ui->lineEdit_pw->text().trimmed();

    if (id.isEmpty()) {
        ui->label_result->setText("ID must be provided");
        return;
    }

    if (pw.isEmpty()) {
        ui->label_result->setText("Password must be provided");
        return;
    }

    ui->lineEdit_id->setEnabled(false);
    ui->lineEdit_pw->setEnabled(false);
    ui->loginBtn->setEnabled(false);

    ui->label_result->setText("");
    mNetworkManager.RequestLogin(id, pw,
        std::bind(&LoginWindow::OnLoginFinished, this, std::placeholders::_1, std::placeholders::_2));
}

void LoginWindow::OnLoginFinished(const bool success, const QString detail)
{
    if (success) {
        ui->label_result->setText("Success");
        LaunchProgram();
    }
    else {
        ui->label_result->setText(detail);
        ui->lineEdit_id->clear();
        ui->lineEdit_pw->clear();

        ui->lineEdit_id->setEnabled(true);
        ui->lineEdit_pw->setEnabled(true);
        ui->loginBtn->setEnabled(true);
    }
}

void LoginWindow::LaunchProgram()
{
    MainWindow* alprClientAppWindow = new MainWindow();
    this->hide();
    alprClientAppWindow->show();
}
