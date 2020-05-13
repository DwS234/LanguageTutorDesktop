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

private:
    Ui::WordSearchResultsDialog *ui;
    QString wordToSearch;
    QSettings* settings;
    QNetworkAccessManager* networkAccessManager;

    const QString BASE_URL = "https://languagetutor-api-1-1589278673698.azurewebsites.net";
    const QRegExp SEARCH_WORD_PATH_REGEXP{"/api/words/all/\\w+/\\w+"};

    void searchWord();
    bool isThisSearchWordResponse(QString path);
    void setWordSearchResults(QJsonArray words);
};

#endif // WORDSEARCHRESULTSDIALOG_H
