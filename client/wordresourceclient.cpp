#include "wordresourceclient.h"
#include "QNetworkReply"
#include "QJsonDocument"
#include "QJsonArray"

WordResourceClient::WordResourceClient()
{
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &WordResourceClient::replyFinished);
}

void WordResourceClient::replyFinished(QNetworkReply* reply){
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    QNetworkRequest request = reply->request();
    QString path = request.url().path();

    if(isThisWordsSearchResponse(path)) {
        if(statusCode == 200) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray wordHintsJsonArray = doc.array();
            QList<QString> wordHints;
            for(QJsonValue v : wordHintsJsonArray)
                wordHints.append(v.toString());
            emit fetchWordHintsDone(OK, wordHints);
        } else {
            emit fetchWordHintsDone(INTERNAL_SERVER_ERROR);
        }
    }
}

void WordResourceClient::fetchWordHints(QString text) {
    QMap<QString, QString> queryParams;
    queryParams.insert("word", text);
    QNetworkRequest request = generateNetworkRequest(WORDS_SEARCH_PATH, queryParams);

    networkAccessManager->get(request);
}

bool WordResourceClient::isThisWordsSearchResponse(QString path) {
    return QString::compare(path, WORDS_SEARCH_PATH) == 0;
}
