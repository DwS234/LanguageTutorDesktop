#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "QSettings"
#include "QNetworkAccessManager"
#include "client/repetititionsresourceclient.h"
#include "model/repetition.h"
#include "mainwindow.h"

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

private:
    Ui::HomeWidget *ui;
    MainWindow* mainWindow;
    QSettings* settings;
    QMap<QString, QVariant> userMap;

    void showLoadingScreen();
    void hideLoadingScreen();
    void showHomeScreen();
    void hideHomeScreen();
    void setWelcomeMessage(QString username);
    void setRepsCountInfo(QString repsCount);
    void setRecentRepetitionTable(QList<Repetition> recentRepetitions);
    void createHeaderItemsForRecentRepsTable();
    void createTableWidgetItemsForRecentRepsTable(QList<Repetition> recentRepetitions);

private slots:
    void onFetchRepetitionsCountDone(RepetititionsResourceClient::ResponseCode responseCode, QString repsCount);
    void onFetchRecentRepetitionsDone(RepetititionsResourceClient::ResponseCode responseCode, QList<Repetition> recentRepetitions);

};

#endif // HOMEWIDGET_H
