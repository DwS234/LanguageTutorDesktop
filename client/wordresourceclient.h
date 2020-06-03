#ifndef WORDRESOURCECLIENT_H
#define WORDRESOURCECLIENT_H

#include "resourceclient.h"
#include "model/word.h"

class WordResourceClient: public ResourceClient
{
    Q_OBJECT
public:
    WordResourceClient();

    enum ResponseCode {OK, INTERNAL_SERVER_ERROR};
    void fetchWordHints(QString text);
    void searchWord(QString foreignMeaningOfWord);

private:
    const QString WORDS_SEARCH_PATH = "/api/words/search";
    const QRegExp SEARCH_WORD_PATH_REGEXP{"/api/words/all/\\w+/\\w+"};

    bool isThisWordsSearchResponse(QString path);
    bool isThisSearchWordResponse(QString path);

private slots:
    void replyFinished(QNetworkReply* reply);
signals:
    void fetchWordHintsDone(ResponseCode code, QList<QString> wordHints=QList<QString>{});
    void searchWordDone(ResponseCode code, QList<Word> words=QList<Word>{});
};

#endif // WORDRESOURCECLIENT_H
