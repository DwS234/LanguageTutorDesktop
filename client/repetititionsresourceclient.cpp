#include "repetititionsresourceclient.h"
#include "QNetworkReply"
#include "QJsonDocument"
#include "QJsonArray"

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

    }
}

void RepetititionsResourceClient::fetchRepetitionsCount() {
    QNetworkRequest request;
    request.setUrl(QUrl(BASE_URL + REPS_COUNT_PATH + "?language=english"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

void RepetititionsResourceClient::fetchRecentRepetitions(int howMany) {
    QNetworkRequest request;
    request.setUrl(QUrl(QString(BASE_URL + RECENT_REPETITIONS_PATH + "?size=%1").arg(howMany)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

bool RepetititionsResourceClient::isThisRepsCountResponse(QString path) {
    return QString::compare(path, REPS_COUNT_PATH) == 0;
}

bool RepetititionsResourceClient::isThisRecentRepsResponse(QString path) {
    return QString::compare(path, RECENT_REPETITIONS_PATH) == 0;
}
