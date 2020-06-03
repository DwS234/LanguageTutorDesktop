#include "repetititionsresourceclient.h"
#include "QNetworkReply"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"

RepetititionsResourceClient::RepetititionsResourceClient()
{
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &RepetititionsResourceClient::replyFinished);
}

void RepetititionsResourceClient::replyFinished(QNetworkReply* reply){
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    QNetworkRequest request = reply->request();
    QString path = request.url().path();
    QNetworkAccessManager::Operation operation = reply->operation();

    if(isThisRepsCountResponse(path)) {
        if(statusCode == 200) {
            QString repetitionsCount{reply->readAll()};
            emit fetchRepetitionsCountDone(ResponseCode::OK, repetitionsCount);
        } else {
            emit fetchRepetitionsCountDone(ResponseCode::INTERNAL_SERVER_ERROR);
        }
    } else if(isThisRecentRepsResponse(path)) {
        if(statusCode == 200) {
            QJsonDocument jsdoc;
            jsdoc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray recentRepetitionsJson = jsdoc.array();
            QList<Repetition> recentRepetitions;
            for(QJsonValue v : recentRepetitionsJson)
                recentRepetitions.append(Repetition{v.toObject()});

            emit fetchRecentRepetitionsDone(ResponseCode::OK, recentRepetitions);
        } else {
            emit fetchRecentRepetitionsDone(ResponseCode::INTERNAL_SERVER_ERROR);
        }

    } else if(isThisDueRepsCountResponse(path)) {
        if(statusCode == 200) {
            QString dueRepetitionsLeft{reply->readAll()};
            int dueRepsLeft = dueRepetitionsLeft.toLong();
            emit fetchDueRepetitionsCountDone(OK, dueRepsLeft);
        } else {
            emit fetchDueRepetitionsCountDone(INTERNAL_SERVER_ERROR);
        }
    } else if(isThisDueRepsResponse(path)) {
        if(statusCode == 200) {
            QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());

            QJsonObject data = jsdoc.object();
            QJsonArray dueRepetitionsJson = data["content"].toArray();
            QList<Repetition> dueRepetitions;
            for(QJsonValue v : dueRepetitionsJson)
                dueRepetitions.append(Repetition{v.toObject()});

            emit fetchDueRepetitionsDone(OK, dueRepetitions);
        } else {
            emit fetchDueRepetitionsDone(INTERNAL_SERVER_ERROR);
        }
    } else if(isThisSetRepResponse(path)) {
        if(statusCode == 200)
            emit(sendRepetitionEvaluationRequestDone(OK));
        else
            emit(sendRepetitionEvaluationRequestDone(INTERNAL_SERVER_ERROR));
    } else if(isThisRepAddedResponse(path, operation)) {
        if(statusCode >=200 && statusCode <= 299)
            emit(addWordToRepsDone(OK));
        else
            emit(addWordToRepsDone(INTERNAL_SERVER_ERROR));
    } else if(isThisRepDeletedResponse(path, operation)) {
        if(statusCode >=200 && statusCode <= 299)
            emit(deleteRepDone(OK));
        else
            emit(deleteRepDone(INTERNAL_SERVER_ERROR));
    }
}

void RepetititionsResourceClient::fetchRepetitionsCount() {
    QMap<QString, QString> queryParams = QMap<QString, QString>{};
    queryParams.insert("language", "english");
    QNetworkRequest request = generateNetworkRequest(REPS_COUNT_PATH, queryParams);

    networkAccessManager->get(request);
}

void RepetititionsResourceClient::fetchRecentRepetitions(int howMany) {
    QMap<QString, QString> queryParams = QMap<QString, QString>{};
    queryParams.insert("size", QString::number(howMany));
    QNetworkRequest request = generateNetworkRequest(RECENT_REPETITIONS_PATH, queryParams);

    networkAccessManager->get(request);
}

void RepetititionsResourceClient::addWordToReps(Word word) {
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("word", word.toQJsonObject());
    doc.setObject(obj);

    QNetworkRequest request = generateNetworkRequest(REP_ADD_PATH);
    networkAccessManager->post(request, doc.toJson());
}

void RepetititionsResourceClient::deleteRep(int wordId) {
    QNetworkRequest request = generateNetworkRequest(QString("/api/repetitions/word/%1").arg(wordId));
    networkAccessManager->deleteResource(request);
}

void RepetititionsResourceClient::fetchDueRepetitions() {
    QNetworkRequest request = generateNetworkRequest(DUE_REPS_PATH);
    networkAccessManager->get(request);
}

void RepetititionsResourceClient::fetchDueRepetitionsCount() {
    QNetworkRequest request = generateNetworkRequest(DUE_REPS_COUNT_PATH);
    networkAccessManager->get(request);
}

void RepetititionsResourceClient::sendRepetitionEvaluationRequest(QString evaluation, int repId) {
    QString path = QString("/api/repetitions/%1/set").arg(repId);

    QMap<QString, QString> queryParams = QMap<QString, QString>{};
    if(QString::compare(evaluation, "0") == 0)
        queryParams.insert("extreme", "");
    else if(QString::compare(evaluation, "1") == 0)
        queryParams.insert("hard", "");
    else if(QString::compare(evaluation, "2") == 0)
        queryParams.insert("pretty-hard", "");
    else if(QString::compare(evaluation, "3") == 0)
        queryParams.insert("medium", "");
    else if(QString::compare(evaluation, "4") == 0)
        queryParams.insert("pretty-easy", "");
    else if(QString::compare(evaluation, "5") == 0)
        queryParams.insert("easy", "");

    QNetworkRequest request = generateNetworkRequest("/api/repetitions/" + QString::number(repId) + "/set", queryParams);

    networkAccessManager->get(request);
}

bool RepetititionsResourceClient::isThisDueRepsCountResponse(QString path) {
    return QString::compare(path, DUE_REPS_COUNT_PATH) == 0;
}

bool RepetititionsResourceClient::isThisDueRepsResponse(QString path) {
    return QString::compare(path, DUE_REPS_PATH) == 0;
}

bool RepetititionsResourceClient::isThisSetRepResponse(QString path) {
    return path.contains(SET_REP_PATH_REGEXP);
}

bool RepetititionsResourceClient::isThisRepAddedResponse(QString path, QNetworkAccessManager::Operation operation) {
    return QString::compare(path, REP_ADD_PATH) == 0 && operation == QNetworkAccessManager::PostOperation;
}

bool RepetititionsResourceClient::isThisRepDeletedResponse(QString path, QNetworkAccessManager::Operation operation) {
    return path.contains(REP_DELETE_PATH_REGEXP) && operation == QNetworkAccessManager::DeleteOperation;
}

bool RepetititionsResourceClient::isThisRepsCountResponse(QString path) {
    return QString::compare(path, REPS_COUNT_PATH) == 0;
}

bool RepetititionsResourceClient::isThisRecentRepsResponse(QString path) {
    return QString::compare(path, RECENT_REPETITIONS_PATH) == 0;
}
