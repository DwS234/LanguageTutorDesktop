#include "authresourceclient.h"
#include "QJsonDocument"
#include "QNetworkReply"
#include "QJsonObject"

AuthResourceClient::AuthResourceClient(QString username, QString password) : username(username), password(password)
{
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &AuthResourceClient::replyFinished);
}

void AuthResourceClient::login() {
    QVariantMap userData;
    userData.insert("username", username);
    userData.insert("password", password);
    QByteArray payload=QJsonDocument::fromVariant(userData).toJson();

    QNetworkRequest request;
    request.setUrl(QUrl("https://languagetutor-api-1-1589278673698.azurewebsites.net/api/auth/signin"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkAccessManager->post(request,payload);
}

void AuthResourceClient::replyFinished(QNetworkReply* reply) {
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = statusCodeV.toInt();
    if(statusCode == 200) {
        qDebug("Status code correct");
        QJsonDocument jsdoc;
        jsdoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsobj = jsdoc.object();
        settings->setValue("accessToken", jsobj["accessToken"].toString());
        settings->setValue("accessTokenExp", jsobj["exp"].toVariant().toLongLong());
        settings->setValue("user", jsobj["user"].toVariant());

        emit loginDone(OK);
    } else if(statusCode == 401) {
        emit loginDone(INVALID_CREDENTIALS);
    } else {
        emit loginDone(INTERNAL_SERVER_ERROR);
    }

    reply->deleteLater();
}
