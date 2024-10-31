#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>

#include "RegisterForm.h"
#include "ui_RegisterForm.h"

#include "Database.h"

RegisterForm::RegisterForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterForm)
{
    ui->setupUi(this);

    setStyles();

    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterForm::onRegisterButtonClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &RegisterForm::onBackButtonClicked);
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::clearData()
{
    ui->nameLineEdit->clear();
    ui->surnameLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->confirmPasswordLineEdit->clear();
}

void RegisterForm::onRegisterButtonClicked()
{
    QString name = ui->nameLineEdit->text();
    QString surname = ui->surnameLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (name.isEmpty() || surname.isEmpty() || email.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        clearData();
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    if (password != confirmPassword) {
        clearData();
        QMessageBox::warning(this, "Ошибка", "Пароль не совпадает!");
        return;
    }

    if (Database::userExists(email)) {
        clearData();
        QMessageBox::warning(this, "Ошибка", QString("Пользователь с email: '%1' уже зарегистрирован!").arg(email));
        return;
    }

    QString query = QString("INSERT INTO Users (name, surname, email, password) VALUES ('%1', '%2', '%3', '%4');")
        .arg(name).arg(surname).arg(email).arg(password);
    Database::executeQuery(query);

    QMessageBox::information(this, "Регистрация", "Регистрация была проведена успешно!");
    clearData();
    emit registerSuccess();
}



void RegisterForm::onBackButtonClicked()
{
    clearData();
    emit back();
}

void RegisterForm::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void RegisterForm::setStyles()
{
    setStyleSheet(R"(
        QWidget {
            font-family: 'JetBrains Mono';
        }
        QWidget#RegisterForm {
            background-color: #e6e6fa;
    }
        QWidget#widget {
            background-color: #f5f5f5;
            border-radius: 30px;
            min-width: 350px;
            max-width: 350px;
            min-height: 665px;
            max-height: 665px;
        }
        QLabel {
            margin-left: 5px;
            font-size: 16px;
            color: #000000;
        }
        QLabel#registerLabel {
            margin-top: 10px;
            margin-bottom: 15px;
            font-size: 16px;
            color: #000000;
        }
        QLineEdit {
            background-color: #ffffff;
            border: 1px solid #d3d3d3;
            border-radius: 8px;
            padding: 8px;
            margin-bottom: 15px;
            font-size: 16px;
            color: #000000;
        }
        QPushButton#registerButton {
            background-color: #333333;
            border-radius: 23px;
            padding: 12px;
            margin-top: 40px;
            margin-bottom: 10px;
            margin-left: 30px;
            margin-right: 30px;
            font-size: 16px;
            color: #ffffff;
        }
        QPushButton#backButton {
            background-color: transparent;
            border: none;
            margin-top: 10px;
            margin-bottom: 10px;
            margin-left: 95px;
            margin-right: 95px;
            font-size: 16px;
            color: #000000;
        }
    )");

    ui->registerLabel->setAlignment(Qt::AlignCenter);
    ui->nameLineEdit->setPlaceholderText("Введите имя");
    ui->surnameLineEdit->setPlaceholderText("Введите фамилию");
    ui->emailLineEdit->setPlaceholderText("example@email.com");
    ui->passwordLineEdit->setPlaceholderText("Введите пароль");
    ui->confirmPasswordLineEdit->setPlaceholderText("Введите пароль");

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
}
