#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include "ui_qt_client.h"

class qt_client : public QMainWindow
{
    Q_OBJECT

public:
    qt_client(QWidget *parent = nullptr);
    ~qt_client();

private:
    Ui::qt_clientClass ui;
    QNetworkAccessManager* manager;
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply;
    QString token;

    QUrl getUrl();

    void onLogin();
    void onLoginReadReady();
    void onLoginFinished();

    void onQuery();
    void onQueryReadReady();
    void onQueryFinished();

    void onSslErrors(const QList<QSslError>& errors);
};
