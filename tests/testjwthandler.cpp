#include "testjwthandler.h"
#include "jwthandler.h"

void TestJWTHandler::testIsTokenValid() {
    JwtHandler jwtHandler{"JWT_TOKEN", 1591740000000};
    QVERIFY(jwtHandler.isTokenValid() == true);
}

void TestJWTHandler::testIsTokenValidWithExpiredJwtToken() {
    JwtHandler jwtHandler{"JWT_TOKEN", 1686348000000};
    QVERIFY(jwtHandler.isTokenValid() == false);
}
