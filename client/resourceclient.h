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
};

#endif // RESOURCECLIENT_H
