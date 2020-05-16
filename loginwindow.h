#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "QSettings"
#include "QNetworkAccessManager"
#include "client/authresourceclient.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    void showLoginScreen();
    void hideLoginScreen();
    void showLoadingScreen();
    void hideLoadingScreen();

private slots:
    void onRegisterButtonClicked();
    void onLoginButtonClicked();
    void onLoginDone(AuthResourceClient::LoginResponseCode responseCode);

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
