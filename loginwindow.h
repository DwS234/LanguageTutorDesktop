#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "QSettings"
#include "QNetworkAccessManager"

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
    void replyFinished(QNetworkReply *reply);

private:
    Ui::LoginWindow *ui;
    QSettings* settings;
    QNetworkAccessManager* networkAccessManager;
};

#endif // LOGINWINDOW_H
