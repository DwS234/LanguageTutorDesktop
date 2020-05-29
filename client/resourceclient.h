#ifndef RESOURCECLIENT_H
#define RESOURCECLIENT_H

#include <QObject>
#include "QNetworkAccessManager"
#include "QSettings"

class ResourceClient : public QObject
{
Q_OBJECT
public:
    explicit ResourceClient(QObject *parent = nullptr);
    ~ResourceClient();
protected:
    const QString BASE_URL;
    QNetworkAccessManager* networkAccessManager;
    QSettings* settings;

    QNetworkRequest generateNetworkRequest(QString path, QMap<QString, QString> queryParams=QMap<QString, QString>{}, bool isAuthRequired=true);
private:
    QString generateQueryString(QMap<QString, QString> queryParams);
};

#endif // RESOURCECLIENT_H
