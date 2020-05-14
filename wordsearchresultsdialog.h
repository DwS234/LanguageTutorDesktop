#ifndef WORDSEARCHRESULTSDIALOG_H
#define WORDSEARCHRESULTSDIALOG_H

#include <QDialog>
#include "QNetworkReply"
#include "QSettings"
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
    void replyFinished(QNetworkReply* reply);
    void onWordAddDeleteClicked();

private:
    Ui::WordSearchResultsDialog *ui;
    QString wordToSearch;
    QSettings* settings;
    QNetworkAccessManager* networkAccessManager;
    QPushButton* lastAddDeleteRepClicked;

    const QString BASE_URL = "https://languagetutor-api-1-1589278673698.azurewebsites.net";
    const QRegExp SEARCH_WORD_PATH_REGEXP{"/api/words/all/\\w+/\\w+"};
    const QString REP_ADD_PATH = "/api/repetitions";
    const QRegExp REP_DELETE_PATH_REGEXP{"/api/repetitions/word/\\d+"};

    void searchWord();
    void addWordToReps(QJsonObject word);
    void deleteRep(int wordId);
    bool isThisSearchWordResponse(QString path);
    bool isThisRepAddedResponse(QString path, QNetworkAccessManager::Operation operation);
    bool isThisRepDeletedResponse(QString path, QNetworkAccessManager::Operation operation);
    void setWordSearchResults(QJsonArray words);
    void hideLoadingScreen();
    void showLoadingScreen(QString text="Ładowanie...");
    void hideSearchResultsScreen();
    void showSearchResultsScreen();
};

#endif // WORDSEARCHRESULTSDIALOG_H
