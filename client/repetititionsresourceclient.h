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
    void addWordToReps(Word word);
    void deleteRep(int wordId);
private:
    const QString REPS_COUNT_PATH = "/api/repetitions/count";
    const QString RECENT_REPETITIONS_PATH = "/api/repetitions/recentlyModfied";
    const QString DUE_REPS_COUNT_PATH = "/api/repetitions/due/count";
    const QString DUE_REPS_PATH = "/api/repetitions/due";
    const QString REP_ADD_PATH = "/api/repetitions";
    const QRegExp SET_REP_PATH_REGEXP{"/api/repetitions/\\d+/set"};
    const QRegExp REP_DELETE_PATH_REGEXP{"/api/repetitions/word/\\d+"};

    bool isThisRepsCountResponse(QString path);
    bool isThisRecentRepsResponse(QString path);
    bool isThisDueRepsCountResponse(QString path);
    bool isThisDueRepsResponse(QString path);
    bool isThisSetRepResponse(QString path);
    bool isThisRepAddedResponse(QString path, QNetworkAccessManager::Operation operation);
    bool isThisRepDeletedResponse(QString path, QNetworkAccessManager::Operation operation);

private slots:
    void replyFinished(QNetworkReply* reply);

signals:
    void fetchRepetitionsCountDone(ResponseCode code, QString repsCount="");
    void fetchRecentRepetitionsDone(ResponseCode code, QList<Repetition> recentRepetitions=QList<Repetition>{});
    void fetchDueRepetitionsDone(ResponseCode code, QList<Repetition> dueRepetitions=QList<Repetition>{});
    void fetchDueRepetitionsCountDone(ResponseCode code, int dueRepsCount=-1);
    void sendRepetitionEvaluationRequestDone(ResponseCode code);
    void addWordToRepsDone(ResponseCode code);
    void deleteRepDone(ResponseCode code);
};

#endif // REPETITITIONSRESOURCECLIENT_H
