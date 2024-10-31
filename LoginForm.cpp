#include <QMessageBox>
#include <QStyleOption>

#include "LoginForm.h"
#include "ui_LoginForm.h"
#include "Database.h"

LoginForm::LoginForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked, this, &LoginForm::onLoginButtonClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginForm::onRegisterButtonClicked);

    setStyles();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::clearData()
{
    ui->emailLineEdit->clear();
    ui->passwordLineEdit->clear();
}

void LoginForm::onLoginButtonClicked()
{
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (email.isEmpty() || password.isEmpty()) {
        clearData();
        QMessageBox::warning(this, "Авторизация", "Все поля должны быть заполнены!");
        return;
    }

    if (!Database::userExists(email)) {
        clearData();
        QMessageBox::warning(this, "Ошибка", QString("Пользователя с email: '%1' не существует!").arg(email));
        return;
    }

    if (!Database::validateUser(email, password)) {
        ui->passwordLineEdit->clear();
        QMessageBox::warning(this, "Ошибка", "Неверный пароль!");
        return;
    }
    QMessageBox::information(this, "Авторизация", "Авторизация прошла успешно!");

    clearData();
    emit loginSuccess();
}

void LoginForm::onRegisterButtonClicked()
{
    clearData();
    emit toRegisterForm();
}

void LoginForm::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void LoginForm::setStyles()
{
    setStyleSheet(R"(
        QWidget {
            font-family: 'JetBrains Mono';
        }
        QWidget#LoginForm {
            background-color: #e6e6fa;
            border: none;
        }
        QWidget#widget {
            background-color: #f5f5f5;
            border-radius: 30px;
            min-width: 350px;
            max-width: 350px;
            min-height: 395px;
            max-height: 395px;
        }
        QLabel {
            margin-left: 5px;
            font-size: 16px;
            color: #000000;
        }
        QLabel#loginLabel {
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
        QPushButton#loginButton {
            background-color: #333333;
            border-radius: 23px;
            padding: 12px;
            margin-top: 40px;
            margin-bottom: 10px;
            margin-left: 70px;
            margin-right: 70px;
            font-size: 16px;
            color: #ffffff;
        }
        QPushButton#registerButton {
            background-color: transparent;
            border: none;
            margin-top: 10px;
            margin-bottom: 10px;
            margin-left: 50px;
            margin-right: 50px;
            font-size: 16px;
            color: #000000;
        }
    )");

    ui->loginLabel->setAlignment(Qt::AlignCenter);

    ui->emailLineEdit->setPlaceholderText("example@email.com");
    ui->passwordLineEdit->setPlaceholderText("Введите пароль");

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}
