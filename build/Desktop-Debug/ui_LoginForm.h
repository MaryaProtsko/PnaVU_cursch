/********************************************************************************
** Form generated from reading UI file 'LoginForm.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINFORM_H
#define UI_LOGINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *loginLabel;
    QLabel *emailLabel;
    QLineEdit *emailLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName("LoginForm");
        LoginForm->resize(800, 600);
        gridLayout = new QGridLayout(LoginForm);
        gridLayout->setObjectName("gridLayout");
        horizontalSpacer = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        widget = new QWidget(LoginForm);
        widget->setObjectName("widget");
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName("gridLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        loginLabel = new QLabel(widget);
        loginLabel->setObjectName("loginLabel");

        verticalLayout->addWidget(loginLabel);

        emailLabel = new QLabel(widget);
        emailLabel->setObjectName("emailLabel");

        verticalLayout->addWidget(emailLabel);

        emailLineEdit = new QLineEdit(widget);
        emailLineEdit->setObjectName("emailLineEdit");

        verticalLayout->addWidget(emailLineEdit);

        passwordLabel = new QLabel(widget);
        passwordLabel->setObjectName("passwordLabel");

        verticalLayout->addWidget(passwordLabel);

        passwordLineEdit = new QLineEdit(widget);
        passwordLineEdit->setObjectName("passwordLineEdit");

        verticalLayout->addWidget(passwordLineEdit);

        loginButton = new QPushButton(widget);
        loginButton->setObjectName("loginButton");

        verticalLayout->addWidget(loginButton);

        registerButton = new QPushButton(widget);
        registerButton->setObjectName("registerButton");

        verticalLayout->addWidget(registerButton);


        gridLayout_2->addLayout(verticalLayout, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 2, 1, 1);


        gridLayout->addWidget(widget, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);


        retranslateUi(LoginForm);

        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QWidget *LoginForm)
    {
        LoginForm->setWindowTitle(QCoreApplication::translate("LoginForm", "Form", nullptr));
        loginLabel->setText(QCoreApplication::translate("LoginForm", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        emailLabel->setText(QCoreApplication::translate("LoginForm", "Email", nullptr));
        passwordLabel->setText(QCoreApplication::translate("LoginForm", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginForm", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginForm", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \320\260\320\272\320\272\320\260\321\203\320\275\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
