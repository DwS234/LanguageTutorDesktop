#ifndef TESTJWTHANDLER_H
#define TESTJWTHANDLER_H

#include <QObject>
#include <QtTest/QtTest>

class TestJWTHandler: public QObject
{
    Q_OBJECT

private slots:
    void testIsTokenValid();
    void testIsTokenValidWithExpiredJwtToken();
};

#endif // TESTJWTHANDLER_H
