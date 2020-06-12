#include "resourceclient.h"
#include "QNetworkReply"
#include "QJsonDocument"

ResourceClient::ResourceClient(QObject *parent) : QObject(parent),
    BASE_URL("https://langtutorapi.azurewebsites.net"),
    networkAccessManager(new QNetworkAccessManager(this)),
    settings(new QSettings("dawid", "LanguageTutor"))
{

}

ResourceClient::~ResourceClient() {
    delete networkAccessManager;
    delete settings;
}

QNetworkRequest ResourceClient::generateNetworkRequest(QString path, QMap<QString, QString> queryParams, bool isAuthRequired) {
    QNetworkRequest request;
    QString query = generateQueryString(queryParams);
    request.setUrl(QUrl(BASE_URL + path + query));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if(isAuthRequired)
        request.setRawHeader(QByteArray("Authorization"), QByteArray(qPrintable("Bearer " + settings->value("accessToken").toString())));

    return request;
}

QString ResourceClient::generateQueryString(QMap<QString, QString> queryParams) {
    QString queryString = "?";
    QMap<QString, QString>::iterator i;
    for(i = queryParams.begin(); i != queryParams.end(); i++)
        queryString += i.key() + "=" + i.value() + "&";
    return queryString;
}


