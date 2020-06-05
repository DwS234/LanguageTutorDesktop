#ifndef WORDSWIDGET_H
#define WORDSWIDGET_H

#include <QWidget>
#include "QPushButton"
#include "QSettings"
#include "QNetworkAccessManager"
#include "QFrame"
#include "client/repetititionsresourceclient.h"
#include "client/wordresourceclient.h"
#include "mainwindow.h"

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
    void onAddWordToRepsDone(RepetititionsResourceClient::ResponseCode);
    void onDeleteRepDone(RepetititionsResourceClient::ResponseCode);
    void onFetchWordsDone(WordResourceClient::ResponseCode, QList<Word>);

private:
    Ui::WordsWidget *ui;
    MainWindow* mainWindow;
    QPushButton* lastAddDeleteRepClicked;
    int currentWordsPage = 0;
    int currentWordsRow = 0;
    int currentWordsCol = 0;

    void hideLoadingScreen();
    void showLoadingScreen(QString text="Ładowanie...");
    void hideWordsScreen();
    void showWordsScreen();
    QFrame* createWordFrame(Word word);
    void createWordFrames(QList<Word> words);
    void hideMoreWordsButton();
};

#endif // WORDSWIDGET_H
