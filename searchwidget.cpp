#include "searchwidget.h"
#include "ui_searchwidget.h"
#include "QNetworkRequest"
#include "QJsonDocument"
#include "QJsonArray"
#include "wordsearchresultsdialog.h"
#include "QMessageBox"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget),
    mainWindow((MainWindow*) QApplication::activeWindow())
{
    ui->setupUi(this);
    connect(ui->searchLineEdit, &QLineEdit::textEdited, this, &SearchWidget::onSearchTextEdited);
    connect(ui->searchHintListWidget, &QListWidget::itemClicked, this, &SearchWidget::onWordHintClicked);
}

SearchWidget::~SearchWidget()
{
    delete ui;
    delete mainWindow;
}

void SearchWidget::onSearchTextEdited(const QString& text) {
    if(text.size() > 2) {
        mainWindow->disableSideMenu();
        WordResourceClient* client = new WordResourceClient;
        connect(client, &WordResourceClient::fetchWordHintsDone, this, &SearchWidget::onFetchWordHintsDone);
        client->fetchWordHints(text);
    } else {
        clearWordHints();
    }
}

void SearchWidget::setWordHints(QList<QString> wordHints) {
    clearWordHints();
    if(wordHints.size() > 0) {
        for(QString wordHint : wordHints) {
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

void SearchWidget::onFetchWordHintsDone(WordResourceClient::ResponseCode code, QList<QString> wordHints) {
    if(code == WordResourceClient::OK) {
        setWordHints(wordHints);
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }

    mainWindow->enableSideMenu();
}

