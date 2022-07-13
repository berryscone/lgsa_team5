#pragma once

#include <QMainWindow>
#include <QString>
#include <QtWidgets/QMainWindow>

#include "AlprClientApp.h"
#include "network/NetworkManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void signalStartNetworkManager();
    void signalStopNetworkManager();
    void signalRequestLogin(QString url, QString id, QString pw);

public slots:
    void OnResponseLoginResult(bool isLoginSuccess);

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    void OnLogin();
    void LaunchProgram();

private:
    Ui::LoginWindow *ui;
    std::unique_ptr<NetworkManager> mNetworkManager;

    void RequestLogin(QString url, QString id, QString pw);
};
