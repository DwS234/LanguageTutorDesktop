#ifndef WORD_H
#define WORD_H

#include "QString"
#include "QList"
#include "QJsonObject"

class Word
{
public:
    Word(const QJsonObject);
    Word(const Word&);
    Word(int id, QString foreign="", QString meaning="", QList<QString> sentences=QList<QString>{}, QString language="english", bool isInRepetition=false);

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
    QJsonObject toQJsonObject();
private:
    int id;
    QString foreign;
    QString meaning;
    QList<QString> sentences;
    QString language;
    bool inRepetition;
};

#endif // WORD_H
