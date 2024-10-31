/********************************************************************************
** Form generated from reading UI file 'RegisterForm.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERFORM_H
#define UI_REGISTERFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterForm
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QLabel *registerLabel;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *surnameLabel;
    QLineEdit *surnameLineEdit;
    QLabel *emailLabel;
    QLineEdit *emailLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QLabel *comfirmPasswordLabel;
    QLineEdit *confirmPasswordLineEdit;
    QPushButton *registerButton;
    QPushButton *backButton;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *RegisterForm)
    {
        if (RegisterForm->objectName().isEmpty())
            RegisterForm->setObjectName("RegisterForm");
        RegisterForm->resize(800, 600);
        gridLayout = new QGridLayout(RegisterForm);
        gridLayout->setObjectName("gridLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 3, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        widget = new QWidget(RegisterForm);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        registerLabel = new QLabel(widget);
        registerLabel->setObjectName("registerLabel");

        verticalLayout->addWidget(registerLabel);

        nameLabel = new QLabel(widget);
        nameLabel->setObjectName("nameLabel");

        verticalLayout->addWidget(nameLabel);

        nameLineEdit = new QLineEdit(widget);
        nameLineEdit->setObjectName("nameLineEdit");

        verticalLayout->addWidget(nameLineEdit);

        surnameLabel = new QLabel(widget);
        surnameLabel->setObjectName("surnameLabel");

        verticalLayout->addWidget(surnameLabel);

        surnameLineEdit = new QLineEdit(widget);
        surnameLineEdit->setObjectName("surnameLineEdit");

        verticalLayout->addWidget(surnameLineEdit);

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

        comfirmPasswordLabel = new QLabel(widget);
        comfirmPasswordLabel->setObjectName("comfirmPasswordLabel");

        verticalLayout->addWidget(comfirmPasswordLabel);

        confirmPasswordLineEdit = new QLineEdit(widget);
        confirmPasswordLineEdit->setObjectName("confirmPasswordLineEdit");

        verticalLayout->addWidget(confirmPasswordLineEdit);

        registerButton = new QPushButton(widget);
        registerButton->setObjectName("registerButton");

        verticalLayout->addWidget(registerButton);

        backButton = new QPushButton(widget);
        backButton->setObjectName("backButton");

        verticalLayout->addWidget(backButton);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        gridLayout->addWidget(widget, 2, 1, 1, 1);


        retranslateUi(RegisterForm);

        QMetaObject::connectSlotsByName(RegisterForm);
    } // setupUi

    void retranslateUi(QWidget *RegisterForm)
    {
        RegisterForm->setWindowTitle(QCoreApplication::translate("RegisterForm", "Form", nullptr));
        registerLabel->setText(QCoreApplication::translate("RegisterForm", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\321\217", nullptr));
        nameLabel->setText(QCoreApplication::translate("RegisterForm", "\320\230\320\274\321\217", nullptr));
        surnameLabel->setText(QCoreApplication::translate("RegisterForm", "\320\244\320\260\320\274\320\270\320\273\320\270\321\217", nullptr));
        emailLabel->setText(QCoreApplication::translate("RegisterForm", "Email", nullptr));
        passwordLabel->setText(QCoreApplication::translate("RegisterForm", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        comfirmPasswordLabel->setText(QCoreApplication::translate("RegisterForm", "\320\237\320\276\320\264\321\202\320\262\320\265\321\200\320\264\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        registerButton->setText(QCoreApplication::translate("RegisterForm", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \320\260\320\272\320\272\320\260\321\203\320\275\321\202", nullptr));
        backButton->setText(QCoreApplication::translate("RegisterForm", "\320\235\320\260\320\267\320\260\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterForm: public Ui_RegisterForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERFORM_H
