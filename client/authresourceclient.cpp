#include "authresourceclient.h"
#include "QJsonDocument"
#include "QNetworkReply"
#include "QJsonObject"
#include "QJsonArray"

AuthResourceClient::AuthResourceClient(QString username, QString password, QString email) : username(username), password(password), email(email)
{
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &AuthResourceClient::replyFinished);
}

void AuthResourceClient::login() {
    QVariantMap userData;
    userData.insert("username", username);
    userData.insert("password", password);
    QByteArray payload=QJsonDocument::fromVariant(userData).toJson();

    QNetworkRequest request = generateNetworkRequest(SIGN_IN_PATH);

    networkAccessManager->post(request,payload);
}

void AuthResourceClient::registerUser() {
    QVariantMap userData;
    userData.insert("username", username);
    userData.insert("password", password);
    userData.insert("email", email);
    QByteArray payload=QJsonDocument::fromVariant(userData).toJson();

    QNetworkRequest request = generateNetworkRequest(SIGN_UP_PATH);

    networkAccessManager->post(request,payload);
}

void AuthResourceClient::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    QNetworkRequest request = reply->request();
    QString path = request.url().path();

    if(isThisSignInResponse(path)) {
        if(statusCode == 200) {
            QJsonDocument jsdoc;
            jsdoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsobj = jsdoc.object();
            settings->setValue("accessToken", jsobj["accessToken"].toString());
            settings->setValue("accessTokenExp", jsobj["exp"].toVariant().toLongLong());
            settings->setValue("user", jsobj["user"].toVariant());

            emit loginDone(OK);
        } else if(statusCode == 401) {
            emit loginDone(INVALID_CREDENTIALS);
        } else if(statusCode == 403) {
            emit loginDone(ACCOUNT_NOT_ACTIVATED);
        }
        else {
            emit loginDone(INTERNAL_SERVER_ERROR);
        }
    } else if(isThisSignUpResponse(path)) {
        if(statusCode == 201) {
            emit registerDone(OK);
        } else if(statusCode == 400) {
//            QJsonDocument jsdoc;
//            jsdoc = QJsonDocument::fromJson(reply->readAll());
//            QJsonObject jsobj = jsdoc.object();
//            QJsonArray validationErrors = jsobj["subErrors"].toArray();
//            QList<QString> validationErrorsList{};
            emit registerDone(BAD_REQUEST);
        } else if(statusCode == 500) {
            emit registerDone(INTERNAL_SERVER_ERROR);
        }
    }

    reply->deleteLater();
}

bool AuthResourceClient::isThisSignInResponse(QString path) {
    return QString::compare(path, SIGN_IN_PATH) == 0;
}

bool AuthResourceClient::isThisSignUpResponse(QString path) {
    return QString::compare(path, SIGN_UP_PATH) == 0;
}
