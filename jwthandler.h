#ifndef JWTHANDLER_H
#define JWTHANDLER_H

#include "QString"

class JwtHandler
{
public:
    JwtHandler(QString jwtToken, qlonglong exp);
    ~JwtHandler();
    bool isTokenValid();

private:
    QString jwtToken;
    qlonglong exp;
};

#endif // JWTHANDLER_H
