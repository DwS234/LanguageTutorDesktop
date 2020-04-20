#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "QNetworkReply"
#include "QSettings"
#include "QNetworkAccessManager"
#include "QListWidgetItem"

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
    void replyFinished(QNetworkReply* reply);
    void onWordHintClicked(QListWidgetItem *wordHint);

private:
    Ui::SearchWidget *ui;
    QSettings* settings;
    QNetworkAccessManager* networkAccessManager;

    const QString BASE_URL = "http://localhost:8081";
    const QString WORDS_SEARCH_PATH = "/api/words/search";

    void fetchWordHints(QString word);
    bool isThisWordsSearchResponse(QString path);
    void setWordHints(QJsonArray wordHints);
    void clearWordHints();
};

#endif // SEARCHWIDGET_H
