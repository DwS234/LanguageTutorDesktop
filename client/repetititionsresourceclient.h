#ifndef REPETITITIONSRESOURCECLIENT_H
#define REPETITITIONSRESOURCECLIENT_H

#include "resourceclient.h"
#include "QJsonArray"
#include "QJsonObject"
#include "model/repetition.h"

class RepetititionsResourceClient : public ResourceClient
{
    Q_OBJECT
public:
    RepetititionsResourceClient();

    enum ResponseCode {OK, INTERNAL_SERVER_ERROR};
    void fetchRepetitionsCount();
    void fetchRecentRepetitions(int howMany);
    void fetchDueRepetitions();
    void fetchDueRepetitionsCount();
    void sendRepetitionEvaluationRequest(QString evaluation, int repId);
private:
    const QString REPS_COUNT_PATH = "/api/repetitions/count";
    const QString RECENT_REPETITIONS_PATH = "/api/repetitions/recentlyModfied";
    const QString DUE_REPS_COUNT_PATH = "/api/repetitions/due/count";
    const QString DUE_REPS_PATH = "/api/repetitions/due";
    const QRegExp SET_REP_PATH_REGEXP{"/api/repetitions/\\d+/set"};

    bool isThisRepsCountResponse(QString path);
    bool isThisRecentRepsResponse(QString path);
    bool isThisDueRepsCountResponse(QString path);
    bool isThisDueRepsResponse(QString path);
    bool isThisSetRepResponse(QString path);


private slots:
    void replyFinished(QNetworkReply* reply);

signals:
    void fetchRepetitionsCountDone(ResponseCode code, QString repsCount="");
    void fetchRecentRepetitionsDone(ResponseCode code, QList<Repetition> recentRepetitions=QList<Repetition>{});
    void fetchDueRepetitionsDone(ResponseCode code, QList<Repetition> dueRepetitions=QList<Repetition>{});
    void fetchDueRepetitionsCountDone(ResponseCode code, int dueRepsCount=-1);
    void sendRepetitionEvaluationRequestDone(ResponseCode code);
};

#endif // REPETITITIONSRESOURCECLIENT_H
