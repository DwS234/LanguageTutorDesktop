#include "word.h"

Word::Word(const QJsonObject object) :
    id(object["id"].toInt()),
    foreign(object["english"].toString()),
    meaning(object["polish"].toString()),
    language(object["language"].toString()),
    inRepetition(object["inRepetition"].toBool())
{
    QString sentencesString = object["sentences"].toString();
    sentences = sentencesString.split("+");
}

int Word::getId() {
    return id;
}

QString Word::getForeign() {
    return foreign;
}

QString Word::getMeaning() {
    return meaning;
}

QList<QString> Word::getSentences() {
    return sentences;
}

QString Word::getLanguage() {
    return language;
}

bool Word::isInRepetition() {
    return inRepetition;
}

void Word::setId(int id) {
    this->id = id;
}

void Word::setForeign(QString foreign) {
    this->foreign = foreign;
}

void Word::setMeaning(QString meaning) {
    this->meaning = meaning;
}

void Word::setSentences(QList<QString> sentences) {
    this->sentences = sentences;
}

void Word::setLanguage(QString language) {
    this->language = language;
}

