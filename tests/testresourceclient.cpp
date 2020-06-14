#include "testresourceclient.h"

void TestResourceClient::testGenerateNetworkRequest() {
    QNetworkRequest request = resourceClient.generateNetworkRequest("/api/test");

    QVERIFY(QString::compare(request.url().toString(), resourceClient.BASE_URL + "/api/test?") == 0);
    QVERIFY(request.rawHeader("Authorization").startsWith("Bearer"));
}

void TestResourceClient::testGenerateNetworkRequestWithQueryParam() {
    QMap<QString, QString> queryParams;
    queryParams.insert("param", "value");
    QNetworkRequest request = resourceClient.generateNetworkRequest("/api/test", queryParams);

    QVERIFY(QString::compare(request.url().toString(), resourceClient.BASE_URL + "/api/test?param=value&") == 0);
}

void TestResourceClient::testGenerateNetworkRequestWithoutAuthorizationNeeded() {
    QMap<QString, QString> queryParams;
    queryParams.insert("param", "value");
    QNetworkRequest request = resourceClient.generateNetworkRequest("/api/test", queryParams, false);
    QVERIFY(QString::compare(request.url().toString(), resourceClient.BASE_URL + "/api/test?param=value&") == 0);
    QVERIFY(QString::compare(request.rawHeader("Authorization"), "") == 0);
}
