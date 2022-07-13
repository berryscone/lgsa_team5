#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "memory.h"

#include <QDebug>
#include <QObject>
#include <QNetworkAccessManager>
#include <QtNetwork>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

signals:
    void Finished();
    void ResponseLoginResult(bool isLoginSuccess);

public slots:
    void OnStart();
    void OnStop();
    void OnRequestLogin(QString url, QString id, QString pw);
    void OnRequestQuery(QString url, QString licensePlate);

private:
    void OnLoginReadReady();
    void OnLoginFinished();

    void OnQueryReadReady();
    void OnQueryFinished();

    void OnSslErrors(const QList<QSslError>& errors);

    std::unique_ptr<QNetworkAccessManager> mManager;
    QQueue<QNetworkReply*> mQueryReplyQueue;
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> mLoginReply;

    QString mToken;
};

#endif // NETWORKMANAGER_H
