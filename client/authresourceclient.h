#ifndef AUTHRESOURCECLIENT_H
#define AUTHRESOURCECLIENT_H

#include "resourceclient.h"

class AuthResourceClient : public ResourceClient
{
    Q_OBJECT
public:
    AuthResourceClient(QString username, QString password);
    enum LoginResponseCode {OK, INVALID_CREDENTIALS, INTERNAL_SERVER_ERROR};
    void login();
private:
    QString username, password;
private slots:
    void replyFinished(QNetworkReply* reply);

signals:
    void loginDone(LoginResponseCode code);
};

#endif // AUTHRESOURCECLIENT_H
