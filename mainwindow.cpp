#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QNetworkReply"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "QMessageBox"
#include "loginwindow.h"
#include "QSizePolicy"
#include "repetitionswidget.h"
#include "homewidget.h"
#include "wordswidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      settings(new QSettings("dawid", "LanguageTutor")),
      networkAccessManager(new QNetworkAccessManager(this))
{
    if(settings->contains("accessToken")) {
       ui->setupUi(this);

       ui->grammar_widget->setVisible(false);
       userMap = settings->value("user").toMap();

       createSideMenu();

       connect(networkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished);
       connect(ui->logoutPushButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);
       connect(ui->sideMenu, &QListWidget::currentItemChanged, this, &MainWindow::onSideMenuCurrentItemChanged);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode >= 200 && statusCode <= 299) {

    } else {
        QMessageBox::warning(this, "Błąd", "Wystąpił błąd. Pracujemy nad tym");
    }
    reply->deleteLater();
}

void MainWindow::onSideMenuCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous) {
    if(previous != nullptr) {
        if(previous == homeItem) {
            ui->main_content_wrapper->removeWidget(currentMainContent);
            currentMainContent->close();
        }
        else if(previous == wordsItem) {
            ui->main_content_wrapper->removeWidget(currentMainContent);
            currentMainContent->close();
        }
        else if(previous == repsItem) {
            ui->main_content_wrapper->removeWidget(currentMainContent);
            currentMainContent->close();
        }
        else if(previous == grammarItem)
            ui->grammar_widget->setVisible(false);
    }

    if(current == homeItem) {
        HomeWidget* homeWidget = new HomeWidget;
        currentMainContent = homeWidget;
        ui->main_content_wrapper->addWidget(homeWidget);
    } else if(current == wordsItem) {
        WordsWidget* wordsWidget = new WordsWidget;
        currentMainContent = wordsWidget;
        ui->main_content_wrapper->addWidget(wordsWidget);
    } else if(current == repsItem) {
          RepetitionsWidget* repsWidget = new RepetitionsWidget;
          currentMainContent = repsWidget;
          ui->main_content_wrapper->addWidget(repsWidget);
    } else if(current == grammarItem) {
        ui->grammar_widget->setVisible(true);
    }
}

void MainWindow::onLogoutButtonClicked() {
    LoginWindow* loginWindow = new LoginWindow();
    settings->clear();
    loginWindow->show();

    hide();
}

void MainWindow::createSideMenu() {
    QListWidget* sideMenu = ui->sideMenu;

    homeItem = new QListWidgetItem(tr("Strona główna"), sideMenu);
    QFont homeItemFont = homeItem->font();
    homeItemFont.setPointSize(12);
    homeItem->setFont(homeItemFont);
    homeItem->setTextAlignment(Qt::AlignCenter);
    homeItem->setSelected(true);
    currentSelectedSideMenuItem = homeItem;

    wordsItem = new QListWidgetItem(tr("Słówka"), sideMenu);
    QFont wordsItemFont = wordsItem->font();
    wordsItemFont.setPointSize(12);
    wordsItem->setFont(wordsItemFont);
    wordsItem->setTextAlignment(Qt::AlignCenter);

    repsItem = new QListWidgetItem(tr("Powtórki"), sideMenu);
    QFont repsItemFont = repsItem->font();
    repsItemFont.setPointSize(12);
    repsItem->setFont(repsItemFont);
    repsItem->setTextAlignment(Qt::AlignCenter);

    grammarItem =  new QListWidgetItem(tr("Gramatyka"), sideMenu);
    QFont grammarItemFont = grammarItem->font();
    grammarItemFont.setPointSize(12);
    grammarItem->setFont(grammarItemFont);
    grammarItem->setTextAlignment(Qt::AlignCenter);
}
