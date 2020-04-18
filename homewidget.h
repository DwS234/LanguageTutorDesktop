#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "QSettings"
#include "QNetworkAccessManager"

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    Ui::HomeWidget *ui;
    QSettings* settings;
    QNetworkAccessManager* networkAccessManager;
    QMap<QString, QVariant> userMap;

    const QString BASE_URL = "http://localhost:8081";
    const QString REPS_COUNT_PATH = "/api/repetitions/count";
    const QString RECENT_REPETITIONS_PATH = "/api/repetitions/recentlyModfied";

    void setWelcomeMessage(QString username);
    void setRepsCountInfo(QString repsCount);
    void setRecentRepetitionTable(QJsonArray recentRepetitions);
    void createHeaderItemsForRecentRepsTable();
    void createTableWidgetItemsForRecentRepsTable(QJsonArray recentRepetitions);
    void fetchRepetitionsCount();
    void fetchRecentRepetitions(int howMany);
    bool isThisRepsCountResponse(QString path);
    bool isThisRecentRepsResponse(QString path);
};

#endif // HOMEWIDGET_H
