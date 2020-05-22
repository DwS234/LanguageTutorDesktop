#ifndef REPETITITIONSRESOURCECLIENT_H
#define REPETITITIONSRESOURCECLIENT_H

#include "resourceclient.h"
#include "QJsonArray"

class RepetititionsResourceClient : public ResourceClient
{
    Q_OBJECT
public:
    RepetititionsResourceClient();

    enum ResponseCode {OK, INTERNAL_SERVER_ERROR};
    void fetchRepetitionsCount();
    void fetchRecentRepetitions(int howMany);
private:
    const QString REPS_COUNT_PATH = "/api/repetitions/count";
    const QString RECENT_REPETITIONS_PATH = "/api/repetitions/recentlyModfied";

    bool isThisRepsCountResponse(QString path);
    bool isThisRecentRepsResponse(QString path);

private slots:
    void replyFinished(QNetworkReply* reply);

signals:
    void fetchRepetitionsCountDone(ResponseCode code, QString repsCount="");
    void fetchRecentRepetitionsDone(ResponseCode code, QJsonArray recentRepetitions=QJsonArray{});
};

#endif // REPETITITIONSRESOURCECLIENT_H
