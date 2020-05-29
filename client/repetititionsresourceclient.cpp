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
            QJsonArray recentRepetitions = jsdoc.array();

            QString repetitionsCount{reply->readAll()};
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

            emit fetchDueRepetitionsDone(OK, data);
        } else {
            emit fetchDueRepetitionsDone(INTERNAL_SERVER_ERROR);
        }
    } else if(isThisSetRepResponse(path)) {
        if(statusCode == 200)
            emit(sendRepetitionEvaluationRequestDone(OK));
        else
            emit(sendRepetitionEvaluationRequestDone(INTERNAL_SERVER_ERROR));
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

bool RepetititionsResourceClient::isThisRepsCountResponse(QString path) {
    return QString::compare(path, REPS_COUNT_PATH) == 0;
}

bool RepetititionsResourceClient::isThisRecentRepsResponse(QString path) {
    return QString::compare(path, RECENT_REPETITIONS_PATH) == 0;
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
