#ifndef WORDRESOURCECLIENT_H
#define WORDRESOURCECLIENT_H

#include "resourceclient.h"

class WordResourceClient: public ResourceClient
{
    Q_OBJECT
public:
    WordResourceClient();

    enum ResponseCode {OK, INTERNAL_SERVER_ERROR};
    void fetchWordHints(QString text);

private:
    const QString WORDS_SEARCH_PATH = "/api/words/search";

    bool isThisWordsSearchResponse(QString path);

private slots:
    void replyFinished(QNetworkReply* reply);
signals:
    void fetchWordHintsDone(ResponseCode code, QList<QString> wordHints=QList<QString>{});
};

#endif // WORDRESOURCECLIENT_H
