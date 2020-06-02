#include "repetition.h"
#include <iostream>

Repetition::Repetition(const QJsonObject object) :
    id(object["id"].toInt()),
    word(Word(object["word"].toObject())),
    nextDate(object["next_date"].toString()),
    easiness(object["easiness"].toDouble()),
    consecutiveCorrectAnswers(object["consecutiveCorrectAnswers"].toInt()),
    howManyTimesSeen(object["howManyTimesSeen"].toInt()),
    createdAt(object["createdAt"].toString())
{}

Repetition::Repetition(const Repetition& repetition):
    id(repetition.id),
    word(repetition.word),
    nextDate(repetition.nextDate),
    easiness(repetition.easiness),
    consecutiveCorrectAnswers(repetition.consecutiveCorrectAnswers),
    howManyTimesSeen(repetition.howManyTimesSeen),
    createdAt(repetition.createdAt)
{};


int Repetition::getId() {
    return id;
}

Word Repetition::getWord() {
    return word;
}

QString Repetition::getNextDate() {
    return nextDate;
}

double Repetition::getEasiness() {
    return easiness;
}

int Repetition::getConsecutiveCorrectAnswers() {
    return consecutiveCorrectAnswers;
}

int Repetition::getHowManyTimesSeen() {
    return howManyTimesSeen;
}

QString Repetition::getCreatedAt() {
    return createdAt;
}

void Repetition::setId(int id) {
    this->id = id;
}

void Repetition::setWord(Word word) {
    this->word = word;
}

void Repetition::setNextDate(QString nextDate) {
    this->nextDate = nextDate;
}

void Repetition::setEasiness(double easiness) {
    this->easiness = easiness;
}

void Repetition::setConsecutiveCorrectAnswers(int consecutiveCorrectAnswers) {
    this->consecutiveCorrectAnswers = consecutiveCorrectAnswers;
}

void Repetition::setHowManyTimesSeen(int howManyTimesSeen) {
    this->howManyTimesSeen = howManyTimesSeen;
}

void Repetition::setCreatedAt(QString createdAt) {
    this->createdAt = createdAt;
}
