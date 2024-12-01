QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    Accesory.cpp \
    FileManager.cpp \
    Function.cpp \
    GraphicDrawer.cpp \
    IDraw.cpp \
    Math.cpp \
    MyGraphicView.cpp \
    Parser.cpp \
    buttonhandler.cpp \
    main.cpp \
    mainwindow.cpp \
    myqlineedit.cpp

HEADERS += \
    Accesory.h \
    FileManager.h \
    Function.h \
    GraphicDrawer.h \
    Header.h \
    IDraw.h \
    MathHeader.h \
    MygraphicsView.h \
    Parser.h \
    buttonhandler.h \
    mainwindow.h \
    myqlineedit.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
