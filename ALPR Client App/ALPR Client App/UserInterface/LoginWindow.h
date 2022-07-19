#pragma once

#include <QMainWindow>
#include <QString>
#include <QtWidgets/QMainWindow>

#include "MainWindow.h"
#include "network/NetworkInterfaces.h"
#include "network/NetworkManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    void OnLogin();
    void LaunchProgram();

public slots:
    void OnLoginFinished(const bool success, const QString detail);

private:
    Ui::LoginWindow *ui;
    ILoginProvider& mLoginProvider;
    const QString mUrl;
};
