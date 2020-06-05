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
#include "QMessageBox"

WordSearchResultsDialog::WordSearchResultsDialog(QString wordToSearch, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordSearchResultsDialog),
    wordToSearch(wordToSearch),
    wordsClient(new WordResourceClient),
    repsClient(new RepetititionsResourceClient)
{
    ui->setupUi(this);
    hideLoadingScreen();
    setWindowTitle(wordToSearch);

    connect(wordsClient, &WordResourceClient::searchWordDone, this, &WordSearchResultsDialog::onSearchWordDone);
    connect(repsClient, &RepetititionsResourceClient::addWordToRepsDone, this, &WordSearchResultsDialog::onAddWordToRepsDone);
    connect(repsClient, &RepetititionsResourceClient::deleteRepDone, this, &WordSearchResultsDialog::onDeleteRepDone);
    wordsClient->searchWord(wordToSearch);
}

WordSearchResultsDialog::~WordSearchResultsDialog()
{
    delete ui;
    delete wordsClient;
    delete repsClient;
}

void WordSearchResultsDialog::setWordSearchResults(QList<Word> words) {
    if(words.size() > 0) {
        int t = 1;
        QVBoxLayout* wrapper = new QVBoxLayout;
        wrapper->setAlignment(Qt::AlignTop);
        for(Word word: words) {
            QHBoxLayout* meaningWrapper = new QHBoxLayout;
            meaningWrapper->setAlignment(Qt::AlignLeft);
            QLabel* meaning = new QLabel{QString("%1. " + word.getMeaning()).arg(t)};
            meaning->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            QFont meaningFont = meaning->font();
            meaningFont.setPointSize(10);
            meaning->setFont(meaningFont);
            meaningWrapper->addWidget(meaning);
            wrapper->addLayout(meaningWrapper);
            t++;

            bool inRepetition = word.isInRepetition();
            QPushButton* addDeleteRepButton = new QPushButton;
            addDeleteRepButton->setProperty("word_id", word.getId());
            addDeleteRepButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            if(inRepetition)
                addDeleteRepButton->setText("-");
            else
                addDeleteRepButton->setText("+");
            connect(addDeleteRepButton, &QPushButton::clicked, this, &WordSearchResultsDialog::onWordAddDeleteClicked);
            meaningWrapper->addWidget(addDeleteRepButton);

            QList<QString> sentences = word.getSentences();
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
        }
        ui->wordResultsWrapper->addLayout(wrapper);
    }
}

void WordSearchResultsDialog::onWordAddDeleteClicked() {
    QPushButton* sender = (QPushButton*) this->sender();
    lastAddDeleteRepClicked = sender;

    int word_id = sender->property("word_id").toInt();
    if(QString::compare(sender->text(), "+") == 0) {
        showLoadingScreen("Trwa dodawanie słowka do systemu powtórek...");
        hideSearchResultsScreen();

        Word word{word_id};
        repsClient->addWordToReps(word);

    } else {
        showLoadingScreen("Trwa usuwanie słówka z systemu powtórek...");
        hideSearchResultsScreen();

        repsClient->deleteRep(word_id);
    }
}

void WordSearchResultsDialog::onAddWordToRepsDone(RepetititionsResourceClient::ResponseCode code) {
    if(code == RepetititionsResourceClient::OK) {
        lastAddDeleteRepClicked->setText("-");
        showSearchResultsScreen();
        hideLoadingScreen();
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void WordSearchResultsDialog::onDeleteRepDone(RepetititionsResourceClient::ResponseCode code) {
    if(code == RepetititionsResourceClient::OK) {
        lastAddDeleteRepClicked->setText("+");
        showSearchResultsScreen();
        hideLoadingScreen();
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void WordSearchResultsDialog::onSearchWordDone(WordResourceClient::ResponseCode code, QList<Word> words) {
    if(code == WordResourceClient::OK) {
        setWordSearchResults(words);
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void WordSearchResultsDialog::showLoadingScreen(QString text) {
    ui->loadingScreen->setVisible(true);
    ui->loadingScreenLabel->setText(text);
}

void WordSearchResultsDialog::hideLoadingScreen() {
    ui->loadingScreen->setVisible(false);
}

void WordSearchResultsDialog::showSearchResultsScreen() {
    ui->scrollArea->setVisible(true);
}

void WordSearchResultsDialog::hideSearchResultsScreen() {
    ui->scrollArea->setVisible(false);
}
