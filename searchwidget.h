#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "QNetworkReply"
#include "QSettings"
#include "QNetworkAccessManager"
#include "QListWidgetItem"
#include "client/wordresourceclient.h"
#include "mainwindow.h"

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

private slots:
    void onSearchTextEdited(const QString& text);
    void onWordHintClicked(QListWidgetItem *wordHint);
    void onFetchWordHintsDone(WordResourceClient::ResponseCode, QList<QString>);

private:
    Ui::SearchWidget *ui;
    MainWindow* mainWindow;
    WordResourceClient* wordsClient;

    void setWordHints(QList<QString> wordHints);
    void clearWordHints();
};

#endif // SEARCHWIDGET_H
