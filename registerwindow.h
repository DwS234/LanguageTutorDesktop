#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include "QNetworkAccessManager"

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
    void replyFinished(QNetworkReply* reply);

private:
    Ui::RegisterWindow *ui;
    QNetworkAccessManager* networkAccessManager;
};

#endif // REGISTERWINDOW_H
