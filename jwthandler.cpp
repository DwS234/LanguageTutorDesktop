#include "jwthandler.h"
#include "QDateTime"

JwtHandler::JwtHandler(QString jwtToken, qlonglong exp): jwtToken(jwtToken), exp(exp)
{
}

JwtHandler::~JwtHandler() {}

bool JwtHandler::isTokenValid() {
    return QDateTime::currentMSecsSinceEpoch() < exp;
}
