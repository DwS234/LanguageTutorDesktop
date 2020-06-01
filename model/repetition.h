#ifndef REPETITION_H
#define REPETITION_H

#include <QString>
#include <QJsonObject>
#include "word.h"

class Repetition
{
public:
    Repetition(const QJsonObject);
    Repetition(const Repetition&);

    int getId();
    Word getWord();
    QString getNextDate();
    double getEasiness();
    int getConsecutiveCorrectAnswers();
    int getHowManyTimesSeen();
    QString getCreatedAt();
    void setId(int id);
    void setWord(Word word);
    void setNextDate(QString nextDate);
    void setEasiness(double easiness);
    void setConsecutiveCorrectAnswers(int consecutiveCorrectAnswers);
    void setHowManyTimesSeen(int howManyTimesSeen);
    void setCreatedAt(QString createdAt);
private:
    int id;
    Word word;
    QString nextDate;
    double easiness;
    int consecutiveCorrectAnswers;
    int howManyTimesSeen;
    QString createdAt;
};

#endif // REPETITION_H
