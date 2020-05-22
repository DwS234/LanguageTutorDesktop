#include "homewidget.h"
#include "ui_homewidget.h"
#include "QNetworkReply"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"
#include "client/repetititionsresourceclient.h"
#include "QMessageBox"

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget),
    settings(new QSettings("dawid", "LanguageTutor"))
{
    ui->setupUi(this);

    userMap = settings->value("user").toMap();

    hideHomeScreen();
    showLoadingScreen();
    setWelcomeMessage(userMap.value("username").toString());

    RepetititionsResourceClient* repsClient = new RepetititionsResourceClient{};
    connect(repsClient, &RepetititionsResourceClient::fetchRepetitionsCountDone, this, &HomeWidget::onFetchRepetitionsCountDone);
    connect(repsClient, &RepetititionsResourceClient::fetchRecentRepetitionsDone, this, &HomeWidget::onFetchRecentRepetitionsDone);
    repsClient->fetchRepetitionsCount();
    repsClient->fetchRecentRepetitions(5);
}

HomeWidget::~HomeWidget()
{
    delete ui;
    delete settings;
}

void HomeWidget::onFetchRepetitionsCountDone(RepetititionsResourceClient::ResponseCode responseCode, QString repsCount) {
    if(responseCode == RepetititionsResourceClient::OK) {
        setRepsCountInfo(repsCount);
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
}

void HomeWidget::onFetchRecentRepetitionsDone(RepetititionsResourceClient::ResponseCode responseCode, QJsonArray recentRepetitions) {
    if(responseCode == RepetititionsResourceClient::OK) {
        setRecentRepetitionTable(recentRepetitions);
    } else {
        QMessageBox::warning(QApplication::activeWindow(), "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }

    hideLoadingScreen();
    showHomeScreen();
}

void HomeWidget::setWelcomeMessage(QString username) {
    ui->welcomeMessageLabel->setText("Witaj, " + username + ", w LanguageTutor");
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

