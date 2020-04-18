#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "loginwindow.h"
#include "QJsonDocument"
#include "QNetworkReply"
#include "QMessageBox"

RegisterWindow::RegisterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow),
    networkAccessManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    ui->register_wrapper->setProperty("cssClass", "auth_wrapper");
    ui->usernameLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->passwordLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->registerButton->setProperty("cssClass", "pushButton-dark");
    ui->registerButton->setCursor(Qt::ClosedHandCursor);
    ui->loginPushButton->setProperty("cssClass", "flatPushButton-dark");
    ui->loginPushButton->setCursor(Qt::ClosedHandCursor);

    connect(ui->loginPushButton, &QPushButton::clicked, this, &RegisterWindow::onLoginButtonClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterButtonClicked);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &RegisterWindow::replyFinished);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode == 200) {
       QMessageBox::information(this, "Informacja", "Utworzono twoje konto. Możesz się teraz zalogować");
    } else if(statusCode == 400) {
        QMessageBox::warning(this, "Błąd", "Podana nazwa użytkownika już istnieje. Spróbuj innej");
    } else {
        QMessageBox::warning(this, "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
    reply->deleteLater();
}

void RegisterWindow::onRegisterButtonClicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    QVariantMap userData;
    userData.insert("username", username);
    userData.insert("password", password);
    QByteArray payload=QJsonDocument::fromVariant(userData).toJson();

    QNetworkRequest request;
    request.setUrl(QUrl("http://localhost:8081/api/auth/signup"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkAccessManager->post(request,payload);
}
void RegisterWindow::onLoginButtonClicked() {
    LoginWindow* loginWindow = new LoginWindow;
    loginWindow->show();

    hide();
}
