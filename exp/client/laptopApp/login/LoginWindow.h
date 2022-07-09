#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtWidgets/QMainWindow>
#include <QtNetwork>
#include <QNetworkAccessManager>

#include "LaptopApp.h"

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
    void onLoginDemo();
    void launchProgram();

private:
    Ui::LoginWindow *ui;

    QNetworkAccessManager* manager;
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;
    QString token;

    QUrl getUrl();

    void onLogin();
    void onLoginReadReady();
    void onLoginFinished();

    void onSslErrors(const QList<QSslError>& errors);
};

#endif // LOGINWINDOW_H
