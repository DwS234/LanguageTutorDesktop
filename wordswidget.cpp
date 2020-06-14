#include "wordswidget.h"
#include "ui_wordswidget.h"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"
#include "QLabel"
#include "QNetworkReply"
#include "QMessageBox"
#include "QFile"

WordsWidget::WordsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordsWidget),
    mainWindow((MainWindow*) QApplication::activeWindow()),
    wordsClient(new WordResourceClient),
    repsClient(new RepetititionsResourceClient)
{
    ui->setupUi(this);
    QFile File(":/stylesheet/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);
    showLoadingScreen();
    hideWordsScreen();

    connect(wordsClient, &WordResourceClient::fetchWordsDone, this, &WordsWidget::onFetchWordsDone);
    connect(repsClient, &RepetititionsResourceClient::addWordToRepsDone, this, &WordsWidget::onAddWordToRepsDone);
    connect(repsClient, &RepetititionsResourceClient::deleteRepDone, this, &WordsWidget::onDeleteRepDone);
    connect(ui->moreWordsPushButton, &QPushButton::clicked, this, &WordsWidget::onMoreWordsButtonClicked);

    mainWindow->disableSideMenu();
    wordsClient->fetchWords(currentWordsPage);
}

WordsWidget::~WordsWidget()
{
    delete ui;
    delete mainWindow;
    delete wordsClient;
    delete repsClient;
}

void WordsWidget::onMoreWordsButtonClicked() {
    currentWordsPage++;
    showLoadingScreen();
    hideWordsScreen();
    mainWindow->disableSideMenu();
    wordsClient->fetchWords(currentWordsPage);
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
        pushButton->setProperty("class", "btn btn-danger");
    } else {
        pushButton->setText("+");
        pushButton->setProperty("class", "btn btn-success");
    }
    QFont pushButtonFont = pushButton->font();
    pushButtonFont.setPointSize(10);
    pushButton->setFont(pushButtonFont);
    pushButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    vBoxLayout->addWidget(pushButton);

    wordFrame->setLayout(vBoxLayout);
    wordFrame->setProperty("class", "card card-dark");

    connect(pushButton, &QPushButton::clicked, this, &WordsWidget::onWordAddDeleteClicked);
    return wordFrame;
}

void WordsWidget::onWordAddDeleteClicked() {
    QPushButton* sender = (QPushButton*) this->sender();
    int word_id = sender->property("word_id").toInt();
    lastAddDeleteRepClicked = sender;

    mainWindow->disableSideMenu();

    if(QString::compare(sender->text(), "+") == 0) {
        showLoadingScreen("Trwa dodawanie słowka do systemu powtórek...");
        hideWordsScreen();

        Word word{word_id};
        repsClient->addWordToReps(word);
    } else {
        showLoadingScreen("Trwa usuwanie słówka z systemu powtórek...");
        hideWordsScreen();

        repsClient->deleteRep(word_id);
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
        lastAddDeleteRepClicked->setProperty("class", "btn btn-danger");
        lastAddDeleteRepClicked->style()->unpolish(lastAddDeleteRepClicked);
        lastAddDeleteRepClicked->style()->polish(lastAddDeleteRepClicked);
        lastAddDeleteRepClicked->update();
        showWordsScreen();
        hideLoadingScreen();
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }

    mainWindow->enableSideMenu();
}

void WordsWidget::onDeleteRepDone(RepetititionsResourceClient::ResponseCode code) {
    if(code == RepetititionsResourceClient::OK) {
        lastAddDeleteRepClicked->setText("+");
        lastAddDeleteRepClicked->setProperty("class", "btn btn-success");
        lastAddDeleteRepClicked->style()->unpolish(lastAddDeleteRepClicked);
        lastAddDeleteRepClicked->style()->polish(lastAddDeleteRepClicked);
        lastAddDeleteRepClicked->update();
        showWordsScreen();
        hideLoadingScreen();
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }

    mainWindow->enableSideMenu();
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

    mainWindow->enableSideMenu();
}


