#include "resourceclient.h"
#include "QNetworkReply"
#include "QJsonDocument"

ResourceClient::ResourceClient(QObject *parent) : QObject(parent),
    BASE_URL("https://languagetutor-api-1-1589278673698.azurewebsites.net"),
    networkAccessManager(new QNetworkAccessManager(this)),
    settings(new QSettings("dawid", "LanguageTutor"))
{

}

ResourceClient::~ResourceClient() {
    delete networkAccessManager;
    delete settings;
}
