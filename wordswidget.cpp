#include "wordswidget.h"
#include "ui_wordswidget.h"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"
#include "QLabel"
#include "QNetworkReply"

WordsWidget::WordsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordsWidget),
    settings(new QSettings("dawid", "LanguageTutor")),
    networkAccessManager(new QNetworkAccessManager(this))

{
    ui->setupUi(this);

    fetchWords();

    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &WordsWidget::replyFinished);
    connect(ui->moreWordsPushButton, &QPushButton::clicked, this, &WordsWidget::onMoreWordsButtonClicked);
}

WordsWidget::~WordsWidget()
{
    delete ui;
}

void WordsWidget::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode >= 200 && statusCode <= 299) {
        QNetworkRequest request = reply->request();
        QNetworkAccessManager::Operation operation = reply->operation();
        QString path = request.url().path();

        if(isThisWordsResponse(path)) {
            QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());

            QJsonObject jsObj = jsdoc.object();
            currentWordsPage = jsObj["pageable"].toObject()["pageNumber"].toInt();
            QJsonArray words = jsObj.value("content").toArray();
            createWordFrames(words);
            if(words.size() == 0)
                hideMoreWordsButton();
        } else if(isThisRepAddedResponse(path, operation)) {
            lastAddDeleteRepClicked->setText("-");
        } else if(isThisRepDeletedResponse(path, operation)) {
            lastAddDeleteRepClicked->setText("+");
        }
    } else {
        // Error. Must be implemented yet.
    }
}

void WordsWidget::onMoreWordsButtonClicked() {
    currentWordsPage++;
    fetchWords();
}


QFrame* WordsWidget::createWordFrame(QJsonObject word) {
    QFrame* wordFrame = new QFrame();

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    vBoxLayout->setMargin(10);

    QLabel* englishLabel = new QLabel(word["english"].toString());
    englishLabel->setMaximumWidth(300);
    englishLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    englishLabel->setWordWrap(true);
    QFont englishLabelFont = englishLabel->font();
    englishLabelFont.setPointSize(11);
    englishLabel->setFont(englishLabelFont);

    vBoxLayout->addWidget(englishLabel);
    englishLabel->setWordWrap(true);
    QLabel* polishLabel = new QLabel(word["polish"].toString());
    polishLabel->setMaximumWidth(300);
    polishLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    polishLabel->setWordWrap(true);
    QFont polishLabelFont = englishLabel->font();
    polishLabelFont.setPointSize(10);
    polishLabel->setFont(polishLabelFont);

    vBoxLayout->addWidget(polishLabel);

    QPushButton* pushButton = new QPushButton();
    pushButton->setProperty("word_id", word["id"].toInt());
    if(word["inRepetition"].toBool()) {
        pushButton->setText("-");
    } else {
        pushButton->setText("+");
    }
    QFont pushButtonFont = pushButton->font();
    pushButtonFont.setPointSize(10);
    pushButton->setFont(pushButtonFont);
    pushButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    vBoxLayout->addWidget(pushButton);

    wordFrame->setLayout(vBoxLayout);
    wordFrame->setStyleSheet("background-color: #242526;");

    connect(pushButton, &QPushButton::clicked, this, &WordsWidget::onWordAddDeleteClicked);
    return wordFrame;
}

void WordsWidget::onWordAddDeleteClicked() {
    QPushButton* sender = (QPushButton*) this->sender();
    int word_id = sender->property("word_id").toInt();
    lastAddDeleteRepClicked = sender;
    if(QString::compare(sender->text(), "+") == 0) {
        QJsonObject word{};
        QJsonObject wordObj{};
        wordObj.insert("id", word_id);
        word.insert("word", wordObj);

        addWordToReps(word);
    } else {
        deleteRep(word_id);
    }
}

void WordsWidget::fetchWords() {
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://localhost:8081/api/words/all/s?page=%1").arg(currentWordsPage)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

bool WordsWidget::isThisWordsResponse(QString path) {
    return QString::compare(path, WORDS_PATH) == 0;
}

bool WordsWidget::isThisRepAddedResponse(QString path, QNetworkAccessManager::Operation operation) {
    return QString::compare(path, REP_ADD_PATH) == 0 && operation == QNetworkAccessManager::PostOperation;
}

bool WordsWidget::isThisRepDeletedResponse(QString path, QNetworkAccessManager::Operation operation) {
    return path.contains(REP_DELETE_PATH_REGEXP) && operation == QNetworkAccessManager::DeleteOperation;
}

void WordsWidget::createWordFrames(QJsonArray words) {
    if(words.size() > 0) {
        foreach(const QJsonValue& value, words) {
            if(currentWordsCol == 2)
            {
                currentWordsCol = 0;
                currentWordsRow++;
            }
            QJsonObject word = value.toObject();

            ui->wordsGrid->addWidget(createWordFrame(word), currentWordsRow, currentWordsCol);
            currentWordsCol++;
        }
    }
}

void WordsWidget::hideMoreWordsButton() {
    ui->moreWordsPushButton->setVisible(false);
}

void WordsWidget::addWordToReps(QJsonObject word) {
    QJsonDocument doc(word);

    QNetworkRequest request;
    request.setUrl(QUrl(BASE_URL + REP_ADD_PATH));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->post(request, doc.toJson());
}

void WordsWidget::deleteRep(int wordId) {
    QNetworkRequest request;
    request.setUrl(QUrl(QString("http://localhost:8081/api/repetitions/word/%1").arg(wordId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->deleteResource(request);
}


