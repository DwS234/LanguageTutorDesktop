#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include "client/authresourceclient.h"

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onRegisterDone(AuthResourceClient::ResponseCode responseCode, QList<QString> validationErrors);

private:
    Ui::RegisterWindow *ui;
    void showLoadingScreen(QString message="Ładowanie...");
    void hideLoadingScreen();
    void showRegisterScreen();
    void hideRegisterScreen();
    void showResultScreen();
    void hideResultScreen();
};

#endif // REGISTERWINDOW_H
