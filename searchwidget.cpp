#include "searchwidget.h"
#include "ui_searchwidget.h"
#include "QNetworkRequest"
#include "QJsonDocument"
#include "QJsonArray"
#include "wordsearchresultsdialog.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget),
    settings(new QSettings("dawid", "LanguageTutor")),
    networkAccessManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &SearchWidget::replyFinished);
    connect(ui->searchLineEdit, &QLineEdit::textEdited, this, &SearchWidget::onSearchTextEdited);
    connect(ui->searchHintListWidget, &QListWidget::itemClicked, this, &SearchWidget::onWordHintClicked);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::onSearchTextEdited(const QString& text) {
    if(text.size() > 2) {
        fetchWordHints(text);
    } else {
        clearWordHints();
    }
}

void SearchWidget::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    QNetworkRequest request = reply->request();
    QNetworkAccessManager::Operation operation = reply->operation();
    QString path = request.url().path();
    if(statusCode >= 200 && statusCode <= 299) {

        qDebug("%s", qPrintable(path));
        if(isThisWordsSearchResponse(path)) {
            QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());

            QJsonArray wordHints = jsdoc.array();
            setWordHints(wordHints);
        }
    } else {
        qDebug("Error code: %d", statusCode);
        qDebug("%s", qPrintable(path));
        qDebug("Some error occured");
    }
    reply->deleteLater();
}

bool SearchWidget::isThisWordsSearchResponse(QString path) {
    return QString::compare(path, WORDS_SEARCH_PATH) == 0;
}

void SearchWidget::fetchWordHints(QString word) {
    QNetworkRequest request;
    request.setUrl(QUrl(QString(BASE_URL + WORDS_SEARCH_PATH + "?word=%1").arg(word)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

void SearchWidget::setWordHints(QJsonArray wordHints) {
    clearWordHints();
    if(wordHints.size() > 0) {
        foreach(const QJsonValue& value, wordHints) {
            QString wordHint = value.toString();

            QListWidgetItem* wordHintItem = new QListWidgetItem;
            wordHintItem->setText(wordHint);
            ui->searchHintListWidget->addItem(wordHintItem);
        }
    } else {
        ui->searchHintListWidget->addItem("Nie znaleziono słówka");
    }
}

void SearchWidget::clearWordHints() {
    ui->searchHintListWidget->clear();
}

void SearchWidget::onWordHintClicked(QListWidgetItem *wordHint) {
    WordSearchResultsDialog* wordSearchResultsDiag = new WordSearchResultsDialog{wordHint->text()};
    wordSearchResultsDiag->show();
}
