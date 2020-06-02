QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client/authresourceclient.cpp \
    client/repetititionsresourceclient.cpp \
    client/resourceclient.cpp \
    client/wordresourceclient.cpp \
    homewidget.cpp \
    jwthandler.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    model/repetition.cpp \
    model/word.cpp \
    registerwindow.cpp \
    repetitionswidget.cpp \
    searchwidget.cpp \
    wordsearchresultsdialog.cpp \
    wordswidget.cpp

HEADERS += \
    client/authresourceclient.h \
    client/repetititionsresourceclient.h \
    client/resourceclient.h \
    client/wordresourceclient.h \
    homewidget.h \
    jwthandler.h \
    loginwindow.h \
    mainwindow.h \
    model/repetition.h \
    model/word.h \
    registerwindow.h \
    repetitionswidget.h \
    searchwidget.h \
    wordsearchresultsdialog.h \
    wordswidget.h

FORMS += \
    homewidget.ui \
    loginwindow.ui \
    mainwindow.ui \
    registerwindow.ui \
    repetitionswidget.ui \
    searchwidget.ui \
    wordsearchresultsdialog.ui \
    wordswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    ../../../Downloads/auth-background.jpg \
    ../../../Downloads/qt-unified-windows-x86-3.2.2-online.exe
