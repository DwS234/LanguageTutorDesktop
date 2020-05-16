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
#include "client/authresourceclient.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    hideLoadingScreen();
    ui->login_wrapper->setProperty("cssClass", "auth_wrapper");
    ui->usernameLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->passwordLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->loginButton->setProperty("cssClass", "pushButton-dark");
    ui->loginButton->setCursor(Qt::ClosedHandCursor);
    ui->registerPushButton->setProperty("cssClass", "flatPushButton-dark");
    ui->registerPushButton->setCursor(Qt::ClosedHandCursor);

    connect(ui->registerPushButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onRegisterButtonClicked() {
   RegisterWindow* registerWindow = new RegisterWindow;
   registerWindow->show();

   hide();
}

void LoginWindow::onLoginButtonClicked()
{
    showLoadingScreen();
    hideLoginScreen();

    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    AuthResourceClient* client = new AuthResourceClient(username, password);
    connect(client, &AuthResourceClient::loginDone, this, &LoginWindow::onLoginDone);
    client->login();
}

void LoginWindow::showLoginScreen() {
    ui->loginScreenn->setVisible(true);
}

void LoginWindow::hideLoginScreen() {
    ui->loginScreenn->setVisible(false);
}

void LoginWindow::showLoadingScreen() {
    ui->loadingScreen->setVisible(true);
}

void LoginWindow::hideLoadingScreen() {
    ui->loadingScreen->setVisible(false);
}

void LoginWindow::onLoginDone(AuthResourceClient::LoginResponseCode responseCode) {
    if(responseCode == AuthResourceClient::LoginResponseCode::OK) {
        MainWindow* mainWindow = new MainWindow;
        mainWindow->show();

        hide();
    } else if(responseCode == AuthResourceClient::LoginResponseCode::INVALID_CREDENTIALS) {
        QMessageBox::warning(this, "Niepoprawne dane logowania", "Niepoprawna nazwa użytkownika lub hasło");
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
    hideLoadingScreen();
    showLoginScreen();
}
