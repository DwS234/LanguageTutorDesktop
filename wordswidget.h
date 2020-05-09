#ifndef WORDSWIDGET_H
#define WORDSWIDGET_H

#include <QWidget>
#include "QPushButton"
#include "QSettings"
#include "QNetworkAccessManager"
#include "QFrame"

namespace Ui {
class WordsWidget;
}

class WordsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WordsWidget(QWidget *parent = nullptr);
    ~WordsWidget();

private slots:
    void onMoreWordsButtonClicked();
    void onWordAddDeleteClicked();
    void replyFinished(QNetworkReply* reply);

private:
    Ui::WordsWidget *ui;
    QPushButton* lastAddDeleteRepClicked;
    QNetworkAccessManager* networkAccessManager;
    QSettings* settings;
    int currentWordsPage = 0;
    int currentWordsRow = 0;
    int currentWordsCol = 0;

    const QString BASE_URL = "http://localhost:8081";
    const QString WORDS_PATH = "/api/words/all/s";
    const QString REP_ADD_PATH = "/api/repetitions";
    const QRegExp REP_DELETE_PATH_REGEXP{"/api/repetitions/word/\\d+"};

    void hideLoadingScreen();
    void showLoadingScreen();
    void hideWordsScreen();
    void showWordsScreen();
    QFrame* createWordFrame(QJsonObject word);
    void createWordFrames(QJsonArray words);
    void addWordToReps(QJsonObject word);
    void deleteRep(int wordId);
    void fetchWords();
    bool isThisWordsResponse(QString path);
    bool isThisRepAddedResponse(QString path, QNetworkAccessManager::Operation operation);
    bool isThisRepDeletedResponse(QString path, QNetworkAccessManager::Operation operation);
    void hideMoreWordsButton();
};

#endif // WORDSWIDGET_H
