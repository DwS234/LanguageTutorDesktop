#include "loginwindow.h"
#include "QFile"
#include "jwthandler.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QFile File(":/stylesheet/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    QApplication a(argc, argv);
    a.setStyleSheet(StyleSheet);

    QSettings* settings = new QSettings("dawid", "LanguageTutor");

    MainWindow mainWindow;
    LoginWindow loginWindow;

    if(settings->contains("accessToken")) {
        qDebug("Access token found");
        JwtHandler jwtHandler{settings->value("accessToken").toString(), settings->value("accessTokenExp").toLongLong()};
        if(jwtHandler.isTokenValid()) {
            qDebug("Access token valid");
            mainWindow.show();
        } else {
            settings->clear();
            loginWindow.show();
        }
    } else {
        loginWindow.show();
    }

    return a.exec();
}
