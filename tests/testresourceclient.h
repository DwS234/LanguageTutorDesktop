#ifndef TESTRESOURCECLIENT_H
#define TESTRESOURCECLIENT_H

#include "client/resourceclient.h"

#include <QtTest/QtTest>

class TestResourceClient: public QObject
{
    Q_OBJECT

private slots:
    void testGenerateNetworkRequest();
    void testGenerateNetworkRequestWithQueryParam();
    void testGenerateNetworkRequestWithoutAuthorizationNeeded();

private:
    ResourceClient resourceClient;
};

#endif // TESTRESOURCECLIENT_H
