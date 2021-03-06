#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSettings"
#include "QNetworkAccessManager"
#include "QListWidgetItem"
#include "QPushButton"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void disableSideMenu();
    void enableSideMenu();

private slots:
    void onLogoutButtonClicked();
    void onSideMenuCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
    Ui::MainWindow *ui;
    QWidget* currentMainContent;
    QSettings* settings;
    QMap<QString, QVariant> userMap;
    QListWidgetItem* homeItem;
    QListWidgetItem* wordsItem;
    QListWidgetItem* repsItem;
    QListWidgetItem* searchItem;
    QListWidgetItem* currentSelectedSideMenuItem;

    void createSideMenu();
};
#endif // MAINWINDOW_H
