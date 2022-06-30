#include "qt_client.h"

qt_client::qt_client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.push_login, &QPushButton::clicked, this, &qt_client::onLogin);
    connect(ui.push_query, &QPushButton::clicked, this, &qt_client::onQuery);

    manager = new QNetworkAccessManager(this);
    // url.setUrl("http://localhost:8888");
    url.setUrl("https://localhost");
}

qt_client::~qt_client()
{
    delete manager;
}


void qt_client::onLogin()
{
    QUrl login_url(url);
    login_url.setPath("/login/");

    auto id = ui.lineedit_id->text();
    auto pw = ui.lineedit_pw->text();
    QString auth = id + ":" + pw;
    QByteArray auth_base64 = auth.toLocal8Bit().toBase64();
    QString basic_auth = "Basic " + auth_base64;

    QNetworkRequest request(login_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", basic_auth.toLocal8Bit());

    QByteArray data;
    reply.reset(manager->post(request, data));
    connect(reply.get(), &QNetworkReply::finished, this, &qt_client::onLoginFinished);
    connect(reply.get(), &QNetworkReply::sslErrors, this, &qt_client::onSslErrors);
    connect(reply.get(), &QIODevice::readyRead, this, &qt_client::onLoginReadReady);
}

void qt_client::onLoginReadReady()
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    token = obj["token"].toString();
    ui.lineedit_token->setText(token);
}

void qt_client::onLoginFinished()
{
}


void qt_client::onQuery()
{
    auto plate_number = ui.lineedit_plate->text();

    QUrlQuery query;
    query.addQueryItem("license-plate-number", plate_number);

    QUrl query_url(url);
    query_url.setQuery(query.query());

    QString token_auth = "Token " + token;

    QNetworkRequest request(query_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setRawHeader("Authorization", token_auth.toLocal8Bit());

    reply.reset(manager->get(request));
    connect(reply.get(), &QNetworkReply::finished, this, &qt_client::onQueryFinished);
    connect(reply.get(), &QIODevice::readyRead, this, &qt_client::onQueryReadReady);
}

void qt_client::onQueryReadReady()
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QString json = doc.toJson(QJsonDocument::Indented);
    ui.textedit_response->setPlainText(json);
}

void qt_client::onQueryFinished()
{

}

void qt_client::onSslErrors(const QList<QSslError>& errors)
{
    reply->ignoreSslErrors();
}
