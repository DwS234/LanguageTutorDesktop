#ifndef REPETITIONSWIDGET_H
#define REPETITIONSWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include "QNetworkAccessManager"
#include "QSettings"
#include "client/repetititionsresourceclient.h"

namespace Ui {
class RepetitionsWidget;
}

class RepetitionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepetitionsWidget(QWidget *parent = nullptr);
    ~RepetitionsWidget();


private slots:
    void onShowAnswerClicked();
    void onHintLetterClicked();
    void onAnswerButtonClicked();
    void onFetchDueRepetitionsDone(RepetititionsResourceClient::ResponseCode code, QList<Repetition> dueRepetitions);
    void onFetchDueRepetitionsCountDone(RepetititionsResourceClient::ResponseCode code, int dueRepsCount);
    void onSendRepetitionEvaluationRequestDone(RepetititionsResourceClient::ResponseCode code);

private:
    Ui::RepetitionsWidget *ui;
    QList<Repetition> dueRepetitions;
    int currentRepIndex = 0;
    int currentRepsCount = -1;

    void hideLoadingScreen();
    void showLoadingScreen();
    void resetUiToDefault();
    void showNoMoreRepsDueMessage();
    void hideNoMoreRepsDueMessage();
    void showRepsFrame();
    void hideRepsFrame();
    void showUserAnswerTextEdit();
    void hideUserAnswerTextEdit();
    void clearUserAnswerTextEdit();
    void showUserAnswer();
    void hideUserAnswer();
    void showCorrectAnswer();
    void hideCorrectAnswer();
    void showHelpButtons();
    void hideHelpButtons();
    void showAnswerButtons();
    void hideAnswerButtons();
    void setDueRepsCountMessage(int dueRepsLeft);
    void setCorrectAnswer(QString correctAnswer);
    QString getCorrectAnswer();
    void setUserAnswer(QString userAnswer);
    QString getUserAnswerFromTextEdit();
    QString getUserAnswer();
    void setEnglishWord(QString word);
    void insertDueRepsToDueRepsList(QList<Repetition> dueRepetitions);
    bool isUserAnswerCorrect(QString userAnswer, QString correctAnswer);
    void setUserAnswerTextEditText(QString text);
};

#endif // REPETITIONSWIDGET_H
