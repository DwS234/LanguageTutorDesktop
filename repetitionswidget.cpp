#include "repetitionswidget.h"
#include "ui_repetitionswidget.h"
#include "QNetworkReply"
#include "QNetworkRequest"
#include "QJsonDocument"
#include "QJsonArray"
#include "QMessageBox"

RepetitionsWidget::RepetitionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepetitionsWidget)
{
    ui->setupUi(this);
    resetUiToDefault();

    showLoadingScreen();
    hideRepsFrame();
    hideNoMoreRepsDueMessage();

    RepetititionsResourceClient* client = new RepetititionsResourceClient;
    connect(client, &RepetititionsResourceClient::fetchDueRepetitionsCountDone, this, &RepetitionsWidget::onFetchDueRepetitionsCountDone);
    connect(client, &RepetititionsResourceClient::fetchDueRepetitionsDone, this, &RepetitionsWidget::onFetchDueRepetitionsDone);
    client->fetchDueRepetitionsCount();
    client->fetchDueRepetitions();

    connect(ui->repsShowAnswerBtn, &QPushButton::clicked, this, &RepetitionsWidget::onShowAnswerClicked);
    connect(ui->repsHintLetterBtn, &QPushButton::clicked, this, &RepetitionsWidget::onHintLetterClicked);
    connect(ui->answerExtremePushButton, &QPushButton::clicked, this, &RepetitionsWidget::onAnswerButtonClicked);
    connect(ui->answerHardPushButton, &QPushButton::clicked, this, &RepetitionsWidget::onAnswerButtonClicked);
    connect(ui->answerPrettyHardPushButton, &QPushButton::clicked, this, &RepetitionsWidget::onAnswerButtonClicked);
    connect(ui->answerMediumPushButton, &QPushButton::clicked, this, &RepetitionsWidget::onAnswerButtonClicked);
    connect(ui->answerPrettyEasyPushButton, &QPushButton::clicked, this, &RepetitionsWidget::onAnswerButtonClicked);
    connect(ui->answerEasyPushButton, &QPushButton::clicked, this, &RepetitionsWidget::onAnswerButtonClicked);
}

RepetitionsWidget::~RepetitionsWidget()
{
    delete ui;
}

void RepetitionsWidget::showNoMoreRepsDueMessage() {
    ui->noMoreDueRepsFrame->setVisible(true);
}

void RepetitionsWidget::hideNoMoreRepsDueMessage() {
    ui->noMoreDueRepsFrame->setVisible(false);
}

void RepetitionsWidget::showRepsFrame() {
    ui->repsFrame->setVisible(true);
}

void RepetitionsWidget::hideRepsFrame() {
    ui->repsFrame->setVisible(false);
}

void RepetitionsWidget::showUserAnswerTextEdit() {
    ui->repsUserAnswerTextEdit->setVisible(true);
}

void RepetitionsWidget::hideUserAnswerTextEdit() {
    ui->repsUserAnswerTextEdit->setVisible(false);
}

void RepetitionsWidget::clearUserAnswerTextEdit() {
    ui->repsUserAnswerTextEdit->clear();
}

void RepetitionsWidget::showUserAnswer() {
    ui->repsUserAnswer->setVisible(true);
}

void RepetitionsWidget::hideUserAnswer() {
    ui->repsUserAnswer->setVisible(false);
}

void RepetitionsWidget::showCorrectAnswer() {
    ui->repsCorrectAnswer->setVisible(true);
}

void RepetitionsWidget::hideCorrectAnswer() {
    ui->repsCorrectAnswer->setVisible(false);
}

void RepetitionsWidget::showHelpButtons() {
    ui->helpButtonsWrapper->setVisible(true);
}

void RepetitionsWidget::hideHelpButtons() {
    ui->helpButtonsWrapper->setVisible(false);
}

void RepetitionsWidget::showAnswerButtons() {
    ui->answerButtonsWrapper->setVisible(true);
}

void RepetitionsWidget::hideAnswerButtons() {
    ui->answerButtonsWrapper->setVisible(false);
}

void RepetitionsWidget::setDueRepsCountMessage(int dueRepsLeft) {
    ui->reps_left_count_label->setText(QString("Pozostało do powtórzenia: %1").arg(dueRepsLeft));
}

void RepetitionsWidget::setCorrectAnswer(QString correctAnswer) {
    ui->repsCorrectAnswer->setText(correctAnswer);
}

QString RepetitionsWidget::getCorrectAnswer() {
    return ui->repsCorrectAnswer->text();
}

void RepetitionsWidget::setEnglishWord(QString word) {
    ui->repsEnglishWord->setText(word);
}

void RepetitionsWidget::insertDueRepsToDueRepsList(QList<Repetition> dueRepetitions) {
    this->dueRepetitions = dueRepetitions;
}

void RepetitionsWidget::setUserAnswer(QString userAnswer) {
    ui->repsUserAnswer->setText(userAnswer);
}

QString RepetitionsWidget::getUserAnswerFromTextEdit() {
    return ui->repsUserAnswerTextEdit->toPlainText().trimmed();
}

QString RepetitionsWidget::getUserAnswer() {
    return ui->repsUserAnswer->text();
}

bool RepetitionsWidget::isUserAnswerCorrect(QString userAnswer, QString correctAnswer) {
    return QString::compare(userAnswer, correctAnswer) == 0;
}

void RepetitionsWidget::setUserAnswerTextEditText(QString text) {
    ui->repsUserAnswerTextEdit->setPlainText(text.trimmed());
}

void RepetitionsWidget::onShowAnswerClicked() {
    setUserAnswer(getUserAnswerFromTextEdit());
    hideUserAnswerTextEdit();

    QString userAnswer = getUserAnswer();
    QString correctAnswer = getCorrectAnswer();

    if(isUserAnswerCorrect(userAnswer, correctAnswer)) {
        ui->repsUserAnswer->setStyleSheet("color: #28a745");
        showUserAnswer();
    } else {
        ui->repsUserAnswer->setStyleSheet("color: #dc3545");
        showUserAnswer();
        showCorrectAnswer();
    }
    hideHelpButtons();
    showAnswerButtons();
}

void RepetitionsWidget::onHintLetterClicked() {
    QString userAnswer = getUserAnswerFromTextEdit();
    QString correctAnswer = getCorrectAnswer();
    if(userAnswer.isEmpty())
        setUserAnswerTextEditText(correctAnswer.left(1));
    else {
        int wrongStartIndex = -1;

        for(int i = 0; i < correctAnswer.size(); i++) {
            if(userAnswer[i] != correctAnswer[i]) {
                wrongStartIndex = i;
                break;
            }
        }

        if(wrongStartIndex != -1)
            setUserAnswerTextEditText(correctAnswer.left(wrongStartIndex + 1));

        if(QString::compare(ui->repsUserAnswerTextEdit->toPlainText(), correctAnswer) == 0) {
            showCorrectAnswer();
            hideUserAnswerTextEdit();
            hideHelpButtons();
            showAnswerButtons();
        }

        ui->repsUserAnswerTextEdit->setFocus();
    }
}

void RepetitionsWidget::onAnswerButtonClicked() {
    QPushButton* clickedButton = (QPushButton*) QObject::sender();
    QString clickedButtonText = clickedButton->text();
    Repetition& currentRep = dueRepetitions.front();

    int repId = currentRep.getId();

    RepetititionsResourceClient* client = new RepetititionsResourceClient;
    connect(client, &RepetititionsResourceClient::sendRepetitionEvaluationRequestDone, this, &RepetitionsWidget::onSendRepetitionEvaluationRequestDone);
    client->sendRepetitionEvaluationRequest(clickedButtonText, repId);
}

void RepetitionsWidget::resetUiToDefault() {
    showUserAnswerTextEdit();
    clearUserAnswerTextEdit();

    hideUserAnswer();
    hideCorrectAnswer();
    showHelpButtons();
    hideAnswerButtons();
    hideNoMoreRepsDueMessage();
}

void RepetitionsWidget::hideLoadingScreen() {
    ui->loadingScreen->setVisible(false);
}

void RepetitionsWidget::showLoadingScreen() {
    ui->loadingScreen->setVisible(true);
}

void RepetitionsWidget::onFetchDueRepetitionsDone(RepetititionsResourceClient::ResponseCode code, QList<Repetition> dueRepetitions) {
    if(code == RepetititionsResourceClient::OK) {
        if(!dueRepetitions.isEmpty()) {
            Repetition& firstRep = dueRepetitions.front();
            Word word = firstRep.getWord();
            setEnglishWord(word.getForeign());
            setCorrectAnswer(word.getMeaning());

            insertDueRepsToDueRepsList(dueRepetitions);
            hideLoadingScreen();
            showRepsFrame();
        } else {
            hideLoadingScreen();
            hideRepsFrame();
            showNoMoreRepsDueMessage();
        }
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void RepetitionsWidget::onFetchDueRepetitionsCountDone(RepetititionsResourceClient::ResponseCode code, int dueRepsCount) {
    if(code == RepetititionsResourceClient::OK) {
        currentRepsCount = dueRepsCount;
        if(dueRepsCount > 0) {
            showRepsFrame();
            hideNoMoreRepsDueMessage();
            setDueRepsCountMessage(dueRepsCount);
        } else {
            showNoMoreRepsDueMessage();
            hideRepsFrame();
        }
        hideLoadingScreen();
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void RepetitionsWidget::onSendRepetitionEvaluationRequestDone(RepetititionsResourceClient::ResponseCode code) {
    qDebug("Done");
    if(code == RepetititionsResourceClient::OK) {
        dueRepetitions.removeFirst();
        setDueRepsCountMessage(--currentRepsCount);
        resetUiToDefault();

        if(dueRepetitions.size() == 0) {
            showLoadingScreen();
            hideRepsFrame();
            RepetititionsResourceClient* client = new RepetititionsResourceClient;
            connect(client, &RepetititionsResourceClient::fetchDueRepetitionsDone, this, &RepetitionsWidget::onFetchDueRepetitionsDone);
            client->fetchDueRepetitions();
        } else {


            Repetition& firstRep = dueRepetitions.front();
            Word word = firstRep.getWord();
            setEnglishWord(word.getForeign());
            setCorrectAnswer(word.getMeaning());
        }
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}
