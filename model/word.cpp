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

Word::Word(int id, QString foreign, QString meaning, QList<QString> sentences, QString language, bool isInRepetition) :
    id(id),
    foreign(foreign),
    meaning(meaning),
    sentences(sentences),
    language(language),
    inRepetition(isInRepetition)
{}

Word::Word(const Word& word) :
    id(word.id),
    foreign(word.foreign),
    meaning(word.meaning),
    sentences(word.sentences),
    language(word.language),
    inRepetition(word.inRepetition)
{};

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

QJsonObject Word::toQJsonObject() {
    QJsonObject wordJsonObject;
    wordJsonObject.insert("id", id);
    wordJsonObject.insert("english", foreign);
    wordJsonObject.insert("polish", meaning);
    wordJsonObject.insert("sentences", sentences.join("+"));
    wordJsonObject.insert("language", language);
    wordJsonObject.insert("inRepetition", inRepetition);

    return wordJsonObject;
}

