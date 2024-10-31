QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:LIBS += -lsqlite3
unix:INCLUDEPATH += /usr/include

SOURCES += \
    Admin.cpp \
    Cart.cpp \
    Category.cpp \
    Customer.cpp \
    Database.cpp \
    LoginForm.cpp \
    MenuForm.cpp \
    Order.cpp \
    Product.cpp \
    RegisterForm.cpp \
    Store.cpp \
    User.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Admin.h \
    Cart.h \
    Category.h \
    Customer.h \
    Database.h \
    LoginForm.h \
    MainWindow.h \
    MenuForm.h \
    Order.h \
    Product.h \
    RegisterForm.h \
    Store.h \
    User.h

FORMS += \
    LoginForm.ui \
    MainWindow.ui \
    MenuForm.ui \
    RegisterForm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
