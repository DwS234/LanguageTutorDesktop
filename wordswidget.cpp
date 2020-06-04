#include "wordswidget.h"
#include "ui_wordswidget.h"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"
#include "QLabel"
#include "QNetworkReply"
#include "QMessageBox"

WordsWidget::WordsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordsWidget)
{
    ui->setupUi(this);
    showLoadingScreen();
    hideWordsScreen();
    WordResourceClient* client = new WordResourceClient;
    connect(client, &WordResourceClient::fetchWordsDone, this, &WordsWidget::onFetchWordsDone);
    client->fetchWords(currentWordsPage);

    connect(ui->moreWordsPushButton, &QPushButton::clicked, this, &WordsWidget::onMoreWordsButtonClicked);
}

WordsWidget::~WordsWidget()
{
    delete ui;
}

void WordsWidget::onMoreWordsButtonClicked() {
    currentWordsPage++;
    showLoadingScreen();
    hideWordsScreen();

    WordResourceClient* client = new WordResourceClient;
    connect(client, &WordResourceClient::fetchWordsDone, this, &WordsWidget::onFetchWordsDone);
    client->fetchWords(currentWordsPage);
}


QFrame* WordsWidget::createWordFrame(Word word) {
    QFrame* wordFrame = new QFrame();

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    vBoxLayout->setMargin(10);

    QLabel* englishLabel = new QLabel(word.getForeign());
    englishLabel->setMaximumWidth(300);
    englishLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    englishLabel->setWordWrap(true);
    QFont englishLabelFont = englishLabel->font();
    englishLabelFont.setPointSize(11);
    englishLabel->setFont(englishLabelFont);

    vBoxLayout->addWidget(englishLabel);
    englishLabel->setWordWrap(true);
    QLabel* polishLabel = new QLabel(word.getMeaning());
    polishLabel->setMaximumWidth(300);
    polishLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    polishLabel->setWordWrap(true);
    QFont polishLabelFont = englishLabel->font();
    polishLabelFont.setPointSize(10);
    polishLabel->setFont(polishLabelFont);

    vBoxLayout->addWidget(polishLabel);

    QPushButton* pushButton = new QPushButton();
    pushButton->setProperty("word_id", word.getId());
    if(word.isInRepetition()) {
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

        showLoadingScreen("Trwa dodawanie słowka do systemu powtórek...");
        hideWordsScreen();

        Word word{word_id};
        RepetititionsResourceClient* client = new RepetititionsResourceClient;
        connect(client, &RepetititionsResourceClient::addWordToRepsDone, this, &WordsWidget::onAddWordToRepsDone);
        client->addWordToReps(word);
    } else {
        showLoadingScreen("Trwa usuwanie słówka z systemu powtórek...");
        hideWordsScreen();

        RepetititionsResourceClient* client = new RepetititionsResourceClient;
        connect(client, &RepetititionsResourceClient::deleteRepDone, this, &WordsWidget::onDeleteRepDone);
        client->deleteRep(word_id);
    }
}

void WordsWidget::createWordFrames(QList<Word> words) {
    if(words.size() > 0) {
        for(Word word : words) {
            if(currentWordsCol == 2)
            {
                currentWordsCol = 0;
                currentWordsRow++;
            }

            ui->wordsGrid->addWidget(createWordFrame(word), currentWordsRow, currentWordsCol);
            currentWordsCol++;
        }
    }
}

void WordsWidget::hideMoreWordsButton() {
    ui->moreWordsPushButton->setVisible(false);
}

void WordsWidget::showLoadingScreen(QString text) {
    ui->loadingScreen->setVisible(true);
    ui->loadingScreenLabel->setText(text);
}

void WordsWidget::hideLoadingScreen() {
    ui->loadingScreen->setVisible(false);
}

void WordsWidget::showWordsScreen() {
    ui->words_widget->setVisible(true);
}

void WordsWidget::hideWordsScreen() {
    ui->words_widget->setVisible(false);
}

void WordsWidget::onAddWordToRepsDone(RepetititionsResourceClient::ResponseCode code) {
    if(code == RepetititionsResourceClient::OK) {
        lastAddDeleteRepClicked->setText("-");
        showWordsScreen();
        hideLoadingScreen();
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void WordsWidget::onDeleteRepDone(RepetititionsResourceClient::ResponseCode code) {
    if(code == RepetititionsResourceClient::OK) {
        lastAddDeleteRepClicked->setText("+");
        showWordsScreen();
        hideLoadingScreen();
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void WordsWidget::onFetchWordsDone(WordResourceClient::ResponseCode code, QList<Word> words) {
    if(code == WordResourceClient::OK) {
        createWordFrames(words);
        if(words.size() == 0)
            hideMoreWordsButton();
        hideLoadingScreen();
        showWordsScreen();
    } else {
       QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}


