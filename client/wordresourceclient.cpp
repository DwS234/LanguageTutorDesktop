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
    } else if(isThisSearchWordResponse(path)) {
        if(statusCode == 200) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray wordsJsonArray = doc.array();
            QList<Word> words;
            for(QJsonValue v : wordsJsonArray)
                words.append(Word(v.toObject()));
            emit(searchWordDone(OK, words));
        } else {
            emit(searchWordDone(INTERNAL_SERVER_ERROR));
        }
    }

    reply->deleteLater();
}

void WordResourceClient::fetchWordHints(QString text) {
    QMap<QString, QString> queryParams;
    queryParams.insert("word", text);
    QNetworkRequest request = generateNetworkRequest(WORDS_SEARCH_PATH, queryParams);

    networkAccessManager->get(request);
}

void WordResourceClient::searchWord(QString foreignMeaningOfWord) {
    QNetworkRequest request = generateNetworkRequest(QString("/api/words/all/english/%1").arg(foreignMeaningOfWord));
    networkAccessManager->get(request);
}

bool WordResourceClient::isThisWordsSearchResponse(QString path) {
    return QString::compare(path, WORDS_SEARCH_PATH) == 0;
}

bool WordResourceClient::isThisSearchWordResponse(QString path) {
    return path.contains(SEARCH_WORD_PATH_REGEXP);
}
