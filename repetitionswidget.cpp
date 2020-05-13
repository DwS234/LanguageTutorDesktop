#include "repetitionswidget.h"
#include "ui_repetitionswidget.h"
#include "QNetworkReply"
#include "QNetworkRequest"
#include "QJsonDocument"
#include "QJsonArray"

RepetitionsWidget::RepetitionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepetitionsWidget),
    settings(new QSettings("dawid", "LanguageTutor")),
    networkAccessManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    hideRepsFrame();
    resetUiToDefault();

    sendDueRepsCountRequest();
    sendDueRepsRequest();

    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &RepetitionsWidget::replyFinished);
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

void RepetitionsWidget::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode >= 200 && statusCode <= 299) {
        QNetworkRequest request = reply->request();
        QNetworkAccessManager::Operation operation = reply->operation();
        QString path = request.url().path();

        if(isThisRepsCountResponse(path)) {
            QString dueRepetitionsLeft{reply->readAll()};
            int dueRepsLeft = dueRepetitionsLeft.toLong();
            currentRepsCount = dueRepsLeft;
            if(dueRepsLeft > 0) {
                showRepsFrame();
                hideNoMoreRepsDueMessage();
                setDueRepsCountMessage(dueRepsLeft);
            } else {
                showNoMoreRepsDueMessage();
                hideRepsFrame();
            }
            hideLoadingScreen();
         } else if(isThisDueRepsResponse(path)) {
            QJsonDocument jsdoc = QJsonDocument::fromJson(reply->readAll());

            QJsonObject jsObj = jsdoc.object();
            currentDueRepsPage = jsObj["pageable"].toObject()["pageNumber"].toInt();
            QJsonArray dueReps = jsObj.value("content").toArray();
            if(dueReps.size() > 0) {
                QJsonObject firstRep = dueReps.at(0).toObject();
                QJsonObject wordObject = firstRep["word"].toObject();
                setEnglishWord(wordObject["english"].toString());
                setCorrectAnswer(wordObject["polish"].toString());

                insertDueRepsJsonArrayToDueRepsList(dueReps);
                hideLoadingScreen();
                showRepsFrame();
            } else {
                hideLoadingScreen();
                hideRepsFrame();
                showNoMoreRepsDueMessage();
            }
        } else if(isThisSetRepResponse(path)) {
            dueRepetitions.removeFirst();

            resetUiToDefault();

            if(dueRepetitions.size() == 0) {
                sendDueRepsRequest();
            } else {
                setDueRepsCountMessage(--currentRepsCount);

                QJsonObject firstRep = dueRepetitions.at(0);
                QJsonObject wordObject = firstRep["word"].toObject();
                setEnglishWord(wordObject["english"].toString());
                setCorrectAnswer(wordObject["polish"].toString());
            }
        }
    } else {
        // Show error message
    }
    reply->deleteLater();
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

bool RepetitionsWidget::isThisRepsCountResponse(QString path) {
    return QString::compare(path, DUE_REPS_COUNT_PATH) == 0;
}

bool RepetitionsWidget::isThisDueRepsResponse(QString path) {
    return QString::compare(path, DUE_REPS_PATH) == 0;
}

bool RepetitionsWidget::isThisSetRepResponse(QString path) {
    return path.contains(SET_REP_PATH_REGEXP);
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

void RepetitionsWidget::insertDueRepsJsonArrayToDueRepsList(QJsonArray dueReps) {
    foreach(const QJsonValue& value, dueReps) {
        QJsonObject repetition = value.toObject();
        dueRepetitions.push_back(repetition);
    }
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
    QJsonObject currentRep = dueRepetitions.at(0);
    int repId = currentRep["id"].toInt();

    QNetworkRequest request;

    if(QString::compare(clickedButtonText, "0") == 0)
        request.setUrl(QUrl(QString("https://languagetutor-api-1-1589278673698.azurewebsites.net/api/repetitions/%1/set?extreme=").arg(repId)));
    else if(QString::compare(clickedButtonText, "1") == 0)
         request.setUrl(QUrl(QString("https://languagetutor-api-1-1589278673698.azurewebsites.net/api/repetitions/%1/set?hard").arg(repId)));
    else if(QString::compare(clickedButtonText, "2") == 0)
        request.setUrl(QUrl(QString("https://languagetutor-api-1-1589278673698.azurewebsites.net/api/repetitions/%1/set?pretty-hard").arg(repId)));
    else if(QString::compare(clickedButtonText, "3") == 0)
        request.setUrl(QUrl(QString("https://languagetutor-api-1-1589278673698.azurewebsites.net/api/repetitions/%1/set?medium").arg(repId)));
    else if(QString::compare(clickedButtonText, "4") == 0)
        request.setUrl(QUrl(QString("https://languagetutor-api-1-1589278673698.azurewebsites.net/api/repetitions/%1/set?pretty-easy").arg(repId)));
    else if(QString::compare(clickedButtonText, "5") == 0)
        request.setUrl(QUrl(QString("https://languagetutor-api-1-1589278673698.azurewebsites.net/api/repetitions/%1/set?easy").arg(repId)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
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

void RepetitionsWidget::sendDueRepsCountRequest() {
    showLoadingScreen();
    QNetworkRequest request;
    request.setUrl(QUrl(BASE_URL + DUE_REPS_COUNT_PATH));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

void RepetitionsWidget::sendDueRepsRequest() {
    showLoadingScreen();
    QNetworkRequest request;
    request.setUrl(QUrl(BASE_URL + DUE_REPS_PATH));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

void RepetitionsWidget::hideLoadingScreen() {
    ui->loadingScreen->setVisible(false);
}

void RepetitionsWidget::showLoadingScreen() {
    ui->loadingScreen->setVisible(true);
}
