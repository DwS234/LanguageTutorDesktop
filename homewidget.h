#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "QSettings"
#include "QNetworkAccessManager"
#include "client/repetititionsresourceclient.h"

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
    QSettings* settings;
    QMap<QString, QVariant> userMap;

    void showLoadingScreen();
    void hideLoadingScreen();
    void showHomeScreen();
    void hideHomeScreen();
    void setWelcomeMessage(QString username);
    void setRepsCountInfo(QString repsCount);
    void setRecentRepetitionTable(QJsonArray recentRepetitions);
    void createHeaderItemsForRecentRepsTable();
    void createTableWidgetItemsForRecentRepsTable(QJsonArray recentRepetitions);

private slots:
    void onFetchRepetitionsCountDone(RepetititionsResourceClient::ResponseCode responseCode, QString repsCount);
    void onFetchRecentRepetitionsDone(RepetititionsResourceClient::ResponseCode responseCode, QJsonArray recentRepetitions);

};

#endif // HOMEWIDGET_H
