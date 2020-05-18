#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "loginwindow.h"
#include "QJsonDocument"
#include "QNetworkReply"
#include "QMessageBox"

RegisterWindow::RegisterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    hideLoadingScreen();
    hideResultScreen();
    ui->register_wrapper->setProperty("cssClass", "auth_wrapper");
    ui->usernameLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->passwordLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->emailLineEdit->setProperty("cssClass", "lineEdit-dark");
    ui->registerButton->setProperty("cssClass", "pushButton-dark");
    ui->registerButton->setCursor(Qt::ClosedHandCursor);
    ui->loginPushButton->setProperty("cssClass", "flatPushButton-dark");
    ui->loginPushButton->setCursor(Qt::ClosedHandCursor);

    connect(ui->loginPushButton, &QPushButton::clicked, this, &RegisterWindow::onLoginButtonClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterButtonClicked);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::onRegisterButtonClicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString email = ui->emailLineEdit->text();

    showLoadingScreen("Trwa proces rejestracji...");
    hideRegisterScreen();

    AuthResourceClient* authClient = new AuthResourceClient(username, password, email);
    connect(authClient, &AuthResourceClient::registerDone, this, &RegisterWindow::onRegisterDone);
    authClient->registerUser();
}
void RegisterWindow::onLoginButtonClicked() {
    LoginWindow* loginWindow = new LoginWindow;
    loginWindow->show();

    hide();
}

void RegisterWindow::onRegisterDone(AuthResourceClient::ResponseCode responseCode, QList<QString> validationErrors) {
    if(responseCode == AuthResourceClient::OK) {
        QMessageBox::information(this, "Informacja", "Utworzono twoje konto. Na twojego maila została wysłana wiadomość z linkiem aktywacyjnym konta.");
        LoginWindow* loginWindow = new LoginWindow;
        loginWindow->show();

        hide();
    } else if(responseCode == AuthResourceClient::BAD_REQUEST) {
        QMessageBox::warning(this, "Błąd", "Niepoprawna nazwa uzytkownika lub email");
    } else if(responseCode == AuthResourceClient::INTERNAL_SERVER_ERROR) {
        QMessageBox::warning(this, "Błąd", "Niespodziewany błąd. Pracujemy nad tym.");
    }
    hideLoadingScreen();
    showRegisterScreen();
}

void RegisterWindow::showLoadingScreen(QString message) {
    ui->loadingScreen->setVisible(true);
    ui->loadingScreenLabel->setText(message);
}

void RegisterWindow::hideLoadingScreen() {
    ui->loadingScreen->setVisible(false);
}

void RegisterWindow::showRegisterScreen() {
    ui->registerScreen->setVisible(true);
}

void RegisterWindow::hideRegisterScreen() {
    ui->registerScreen->setVisible(false);
}

void RegisterWindow::showResultScreen() {
    ui->resultScreen->setVisible(true);
}

void RegisterWindow::hideResultScreen() {
    ui->resultScreen->setVisible(false);
}
