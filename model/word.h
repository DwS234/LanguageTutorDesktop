#ifndef WORD_H
#define WORD_H

#include "QString"
#include "QList"
#include "QJsonObject"

class Word
{
public:
    Word(const QJsonObject);

    int getId();
    QString getForeign();
    QString getMeaning();
    QList<QString> getSentences();
    QString getLanguage();
    bool isInRepetition();
    void setId(int id);
    void setForeign(QString foreign);
    void setMeaning(QString meaning);
    void setSentences(QList<QString> sentences);
    void setLanguage(QString lanugage);
private:
    int id;
    QString foreign;
    QString meaning;
    QList<QString> sentences;
    QString language;
    bool inRepetition;
};

#endif // WORD_H
