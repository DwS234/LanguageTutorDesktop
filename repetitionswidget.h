#ifndef REPETITIONSWIDGET_H
#define REPETITIONSWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include "QNetworkAccessManager"
#include "QSettings"

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
    void replyFinished(QNetworkReply* reply);
    void onShowAnswerClicked();
    void onHintLetterClicked();
    void onAnswerButtonClicked();

private:
    Ui::RepetitionsWidget *ui;
    QSettings* settings;
    QNetworkAccessManager* networkAccessManager;
    QList<QJsonObject> dueRepetitions;
    int currentRepIndex = 0;
    int currentDueRepsPage;
    int currentRepsCount = -1;
    const QString BASE_URL = "http://localhost:8081";
    const QString DUE_REPS_COUNT_PATH = "/api/repetitions/due/count";
    const QString DUE_REPS_PATH = "/api/repetitions/due";
    const QRegExp SET_REP_PATH_REGEXP{"/api/repetitions/\\d+/set"};

    void hideLoadingScreen();
    void showLoadingScreen();
    void resetUiToDefault();
    void showNoMoreRepsDueMessage();
    void hideNoMoreRepsDueMessage();
    void showRepsFrame();
    void hideRepsFrame();
    void fetchDueRepetitions();
    void fetchDueRepetitionsCount();
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
    bool isThisRepsCountResponse(QString path);
    bool isThisDueRepsResponse(QString path);
    bool isThisSetRepResponse(QString path);
    void sendDueRepsCountRequest();
    void sendDueRepsRequest();
    void setCorrectAnswer(QString correctAnswer);
    QString getCorrectAnswer();
    void setUserAnswer(QString userAnswer);
    QString getUserAnswerFromTextEdit();
    QString getUserAnswer();
    void setEnglishWord(QString word);
    void insertDueRepsJsonArrayToDueRepsList(QJsonArray dueReps);
    bool isUserAnswerCorrect(QString userAnswer, QString correctAnswer);
    void setUserAnswerTextEditText(QString text);
};

#endif // REPETITIONSWIDGET_H
