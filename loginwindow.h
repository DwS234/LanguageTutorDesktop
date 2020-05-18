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
private slots:
    void onRegisterButtonClicked();
    void onLoginButtonClicked();
    void onLoginDone(AuthResourceClient::ResponseCode responseCode);

private:
    Ui::LoginWindow *ui;
    void showLoginScreen();
    void hideLoginScreen();
    void showLoadingScreen();
    void hideLoadingScreen();
};

#endif // LOGINWINDOW_H
