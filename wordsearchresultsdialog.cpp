#include "wordsearchresultsdialog.h"
#include "ui_wordsearchresultsdialog.h"
#include "QJsonArray"
#include "QJsonDocument"
#include "QJsonObject"
#include "QTreeWidget"
#include "QTreeWidgetItem"
#include "QLabel"
#include "QPushButton"
#include "QSizePolicy"

WordSearchResultsDialog::WordSearchResultsDialog(QString wordToSearch, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordSearchResultsDialog),
    wordToSearch(wordToSearch),
    settings(new QSettings("dawid", "LanguageTutor")),
    networkAccessManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    setWindowTitle(wordToSearch);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &WordSearchResultsDialog::replyFinished);

    searchWord();
}

WordSearchResultsDialog::~WordSearchResultsDialog()
{
    delete ui;
}

void WordSearchResultsDialog::searchWord() {
    QNetworkRequest request;
    request.setUrl(QUrl(QString(BASE_URL + "/api/words/all/english/%1").arg(wordToSearch)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

void WordSearchResultsDialog::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode >= 200 && statusCode <= 299) {
        QNetworkRequest request = reply->request();
        QNetworkAccessManager::Operation operation = reply->operation();
        QString path = request.url().path();
        qDebug("%s", qPrintable(path));

        if(isThisSearchWordResponse(path)) {
             QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());
             QJsonArray words = jsdoc.array();

             setWordSearchResults(words);
        }
    } else {
        qDebug("Some error occured");
    }
    reply->deleteLater();
}

bool WordSearchResultsDialog::isThisSearchWordResponse(QString path) {
    return path.contains(SEARCH_WORD_PATH_REGEXP);
}

void WordSearchResultsDialog::setWordSearchResults(QJsonArray words) {
    if(words.size() > 0) {
        int t = 1;
        QVBoxLayout* wrapper = new QVBoxLayout;
        wrapper->setAlignment(Qt::AlignTop);
        foreach(const QJsonValue& value, words) {
            QJsonObject word = value.toObject();

            QHBoxLayout* meaningWrapper = new QHBoxLayout;
            meaningWrapper->setAlignment(Qt::AlignLeft);
            QLabel* meaning = new QLabel{QString("%1. " + word["polish"].toString()).arg(t)};
            meaning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            QFont meaningFont = meaning->font();
            meaningFont.setPointSize(10);
            meaning->setFont(meaningFont);
            meaningWrapper->addWidget(meaning);
            wrapper->addLayout(meaningWrapper);
            t++;

            bool inRepetition = word["inRepetition"].toBool();
            QPushButton* addDeleteRepButton = new QPushButton;
            addDeleteRepButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            if(inRepetition)
                addDeleteRepButton->setText("-");
            else
                addDeleteRepButton->setText("+");
            meaningWrapper->addWidget(addDeleteRepButton);

            QList<QString> sentences = word["sentences"].toString().split("+");
            if(sentences.size() > 0) {
                for(int i = 0; i < sentences.size(); i++) {
                    QLabel* sentenceLabel = new QLabel;
                    sentenceLabel->setText(sentences.at(i));
                    QFont sentenceFont = meaning->font();
                    sentenceFont.setPointSize(8);
                    sentenceLabel->setFont(sentenceFont);
                    wrapper->addWidget(sentenceLabel);
                    sentenceLabel->setStyleSheet("margin-left: 20px");
                }
            }

//                   item->addChild(new QTreeWidgetItem{sentences.at(i)})
//            }
        }
        ui->wordResultsWrapper->addLayout(wrapper);
    } else {

    }
}
