#ifndef AUTHRESOURCECLIENT_H
#define AUTHRESOURCECLIENT_H

#include "resourceclient.h"
#include "QJsonDocument"

class AuthResourceClient : public ResourceClient
{
    Q_OBJECT
public:
    AuthResourceClient(QString username, QString password, QString email="");
    enum ResponseCode {OK, INVALID_CREDENTIALS, BAD_REQUEST, INVALID_REGISTRATION_TOKEN, INTERNAL_SERVER_ERROR, ACCOUNT_NOT_ACTIVATED};

    void login();
    void registerUser();
private:
    QString username, password, email;
    const QString SIGN_IN_PATH = "/api/auth/signin";
    const QString SIGN_UP_PATH = "/api/auth/signup";
    bool isThisSignInResponse(QString path);
    bool isThisSignUpResponse(QString path);
private slots:
    void replyFinished(QNetworkReply* reply);

signals:
    void loginDone(ResponseCode code);
    void registerDone(ResponseCode code, QList<QString> validationErrors=QList<QString>{});
};

#endif // AUTHRESOURCECLIENT_H
