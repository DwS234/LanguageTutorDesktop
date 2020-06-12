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
#include "searchwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      settings(new QSettings("dawid", "LanguageTutor"))
{
   ui->setupUi(this);

   userMap = settings->value("user").toMap();

   createSideMenu();

   connect(ui->logoutPushButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);
   connect(ui->sideMenu, &QListWidget::currentItemChanged, this, &MainWindow::onSideMenuCurrentItemChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
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
        else if(previous == searchItem) {
            ui->main_content_wrapper->removeWidget(currentMainContent);
            currentMainContent->close();
        }
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
    } else if(current == searchItem) {
        SearchWidget* searchWidget = new SearchWidget;
        currentMainContent = searchWidget;
        ui->main_content_wrapper->addWidget(searchWidget);
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

    searchItem =  new QListWidgetItem(tr("Szukaj"), sideMenu);
    QFont searchItemFont = searchItem->font();
    searchItemFont.setPointSize(12);
    searchItem->setFont(searchItemFont);
    searchItem->setTextAlignment(Qt::AlignCenter);
}

void MainWindow::disableSideMenu() {
    ui->sideMenu->setEnabled(false);
    ui->verticalWidget->setCursor(Qt::ForbiddenCursor);
}

void MainWindow::enableSideMenu() {
    ui->sideMenu->setEnabled(true);
    ui->verticalWidget->setCursor(Qt::ArrowCursor);
}
