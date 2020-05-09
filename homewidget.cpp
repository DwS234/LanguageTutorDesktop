#include "homewidget.h"
#include "ui_homewidget.h"
#include "QNetworkReply"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget),
    settings(new QSettings("dawid", "LanguageTutor")),
    networkAccessManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    userMap = settings->value("user").toMap();

    hideHomeScreen();
    setWelcomeMessage(userMap.value("username").toString());
    fetchRepetitionsCount();
    fetchRecentRepetitions(5);

    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &HomeWidget::replyFinished);
}

HomeWidget::~HomeWidget()
{
    delete ui;
}

void HomeWidget::setWelcomeMessage(QString username) {
    ui->welcomeMessageLabel->setText("Witaj, " + username + ", w LanguageTutor");
}

void HomeWidget::fetchRepetitionsCount() {
    QNetworkRequest request;
    request.setUrl(QUrl(BASE_URL + REPS_COUNT_PATH + "?language=english"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

void HomeWidget::fetchRecentRepetitions(int howMany=5) {
    showLoadingScreen();
    QNetworkRequest request;
    request.setUrl(QUrl(QString(BASE_URL + RECENT_REPETITIONS_PATH + "?size=%1").arg(howMany)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));
    networkAccessManager->get(request);
}

void HomeWidget::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode >= 200 && statusCode <= 299) {
        QNetworkRequest request = reply->request();
        QNetworkAccessManager::Operation operation = reply->operation();
        QString path = request.url().path();

        if(isThisRepsCountResponse(path)) {
            QString repetitionsCount{reply->readAll()};
            setRepsCountInfo(repetitionsCount);
        } else if(isThisRecentRepsResponse(path)) {
            QJsonDocument jsdoc;
            jsdoc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray recentRepetitions = jsdoc.array();

            setRecentRepetitionTable(recentRepetitions);
            hideLoadingScreen();
            showHomeScreen();
        }
    } else {
        //Error handling. Still figuring out how to implement this
    }

    reply->deleteLater();
}

bool HomeWidget::isThisRepsCountResponse(QString path) {
    return QString::compare(path, REPS_COUNT_PATH) == 0;
}

bool HomeWidget::isThisRecentRepsResponse(QString path) {
    return QString::compare(path, RECENT_REPETITIONS_PATH) == 0;
}

void HomeWidget::setRepsCountInfo(QString repsCount) {
    long repsCountL = repsCount.toLong();
    if(repsCountL == 0)
        ui->repetitionsCountInfoLabel->setText("Nie masz narazie żadnych nauczonych słówek");
    else
        ui->repetitionsCountInfoLabel->setText("Twoja ogólna liczba nauczonych słówek to: " + repsCount + ". Gratulacje!");
}

void HomeWidget::setRecentRepetitionTable(QJsonArray recentRepetitions) {
    if(recentRepetitions.size() > 0) {

        ui->recentRepetitionsFrame->setVisible(true);
        ui->recentRepetitionsTable->setRowCount(recentRepetitions.size());

        createHeaderItemsForRecentRepsTable();
        createTableWidgetItemsForRecentRepsTable(recentRepetitions);
    }
}

void HomeWidget::createHeaderItemsForRecentRepsTable() {
    ui->recentRepetitionsTable->setColumnCount(4);
    ui->recentRepetitionsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Angielski"));
    ui->recentRepetitionsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Polski"));
    ui->recentRepetitionsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Następna powtórka"));
    ui->recentRepetitionsTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Liczba dotychczasowych powtórek"));
}

void HomeWidget::createTableWidgetItemsForRecentRepsTable(QJsonArray recentRepetitions) {
    int row = 0;

    foreach(const QJsonValue& value, recentRepetitions) {
        QJsonObject obj = value.toObject();
        QJsonObject word = obj["word"].toObject();

        QTableWidgetItem* englishItem = new QTableWidgetItem(word["english"].toString());
        englishItem->setTextAlignment(Qt::AlignCenter);
        ui->recentRepetitionsTable->setItem(row, 0, englishItem);

        QTableWidgetItem* polishItem = new QTableWidgetItem(word["polish"].toString());
        polishItem->setTextAlignment(Qt::AlignCenter);
        ui->recentRepetitionsTable->setItem(row, 1, polishItem);

        QTableWidgetItem* nextDateItem = new QTableWidgetItem(obj["next_date"].toString());
        nextDateItem->setTextAlignment(Qt::AlignCenter);
        ui->recentRepetitionsTable->setItem(row, 2, nextDateItem);

        QTableWidgetItem* howManyTimesSeenItem = new QTableWidgetItem(QString::number(obj["howManyTimesSeen"].toInt()));
        howManyTimesSeenItem->setTextAlignment(Qt::AlignCenter);
        qDebug("%d", obj["howManyTimesSeen"].toInt());
        ui->recentRepetitionsTable->setItem(row, 3, howManyTimesSeenItem);
        row++;
    }
}

void HomeWidget::hideLoadingScreen() {
    ui->loadingScreen->setVisible(false);
}

void HomeWidget::showLoadingScreen() {
    ui->loadingScreen->setVisible(true);
}

void HomeWidget::showHomeScreen() {
    ui->home_widget->setVisible(true);
}

void HomeWidget::hideHomeScreen() {
    ui->home_widget->setVisible(false);
}

