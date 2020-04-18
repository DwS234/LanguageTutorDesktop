#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "registerwindow.h"
#include "mainwindow.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QNetworkReply"
#include "QMessageBox"
#include "QRegularExpressionValidator"
#include "QRegularExpression"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow),
    settings(new QSettings("dawid", "LanguageTutor")),
    networkAccessManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    ui->login_wrapper->setProperty("cssClass", "auth_wrapper");
    ui->usernameLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->passwordLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->loginButton->setProperty("cssClass", "pushButton-dark");
    ui->loginButton->setCursor(Qt::ClosedHandCursor);
    ui->registerPushButton->setProperty("cssClass", "flatPushButton-dark");
    ui->registerPushButton->setCursor(Qt::ClosedHandCursor);

    connect(ui->registerPushButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &LoginWindow::replyFinished);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode == 200) {
        qDebug("Status code correct");
        QJsonDocument jsdoc;
        jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        settings->setValue("accessToken", jsobj["accessToken"].toString());
        settings->setValue("accessTokenExp", jsobj["exp"].toVariant().toLongLong());
        settings->setValue("user", jsobj["user"].toVariant());

        MainWindow* mainWindow = new MainWindow;
        mainWindow->show();

        hide();
    } else if(statusCode == 401) {
        QMessageBox::warning(this, "Niepoprawne dane logowania", "Niepoprawna nazwa użytkownika lub hasło");
    } else {
        QMessageBox::warning(this, "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
    reply->deleteLater();
}

void LoginWindow::onRegisterButtonClicked() {
   RegisterWindow* registerWindow = new RegisterWindow;
   registerWindow->show();

   hide();
}

void LoginWindow::onLoginButtonClicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    QVariantMap userData;
    userData.insert("username", username);
    userData.insert("password", password);
    QByteArray payload=QJsonDocument::fromVariant(userData).toJson();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:8081/api/auth/signin"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkAccessManager->post(request,payload);
}
