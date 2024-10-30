/********************************************************************************
** Form generated from reading UI file 'field.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIELD_H
#define UI_FIELD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_field
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *field)
    {
        if (field->objectName().isEmpty())
            field->setObjectName("field");
        field->resize(800, 600);
        centralwidget = new QWidget(field);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(190, 170, 381, 161));
        label->setStyleSheet(QString::fromUtf8("font-size: 35px;\n"
""));
        field->setCentralWidget(centralwidget);
        menubar = new QMenuBar(field);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        field->setMenuBar(menubar);
        statusbar = new QStatusBar(field);
        statusbar->setObjectName("statusbar");
        field->setStatusBar(statusbar);

        retranslateUi(field);

        QMetaObject::connectSlotsByName(field);
    } // setupUi

    void retranslateUi(QMainWindow *field)
    {
        field->setWindowTitle(QCoreApplication::translate("field", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("field", "\320\242\321\203\321\202 \320\261\321\203\320\264\320\265\321\202 \321\201\321\203\320\264\320\276\320\272\321\203, \320\272\321\201\321\202\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class field: public Ui_field {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIELD_H
