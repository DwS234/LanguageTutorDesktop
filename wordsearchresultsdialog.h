#ifndef WORDSEARCHRESULTSDIALOG_H
#define WORDSEARCHRESULTSDIALOG_H

#include <QDialog>
#include "QNetworkReply"
#include "QSettings"
#include "client/repetititionsresourceclient.h"
#include "client/wordresourceclient.h"

namespace Ui {
class WordSearchResultsDialog;
}

class WordSearchResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WordSearchResultsDialog(QString wordToSearch, QWidget *parent = nullptr);
    ~WordSearchResultsDialog();

private slots:
    void onWordAddDeleteClicked();
    void onAddWordToRepsDone(RepetititionsResourceClient::ResponseCode);
    void onDeleteRepDone(RepetititionsResourceClient::ResponseCode);
    void onSearchWordDone(WordResourceClient::ResponseCode, QList<Word>);

private:
    Ui::WordSearchResultsDialog *ui;
    QString wordToSearch;
    QPushButton* lastAddDeleteRepClicked;
    WordResourceClient* wordsClient;
    RepetititionsResourceClient* repsClient;

    void setWordSearchResults(QList<Word> words);
    void hideLoadingScreen();
    void showLoadingScreen(QString text="Ładowanie...");
    void hideSearchResultsScreen();
    void showSearchResultsScreen();
};

#endif // WORDSEARCHRESULTSDIALOG_H
