#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    ui->mainWidgets->setFixedSize(1581,761);
    ui->infoWidgets->setFixedSize(1581,121);
    ui->infoWidgets->setCurrentWidget(ui->mainPage);
    ui->mainWidgets->setCurrentWidget(ui->startPage);
    ui->groupBox_4->setTitle("Регистрация");
    ui->groupBox_5->setTitle("Авторизация");
}

mainWidget::~mainWidget()
{
    delete ui;
}

QString mainWidget::getDay()
{
    QDate currentDate = QDate::currentDate();
    QString dayName = currentDate.toString("dddd");

    return dayName;
}

void mainWidget::on_pushButton_clicked()
{
    ui->infoWidgets->setCurrentWidget(ui->page_4);
}


void mainWidget::on_pushButtonBackInfo_clicked()
{
    ui->infoWidgets->setCurrentWidget(ui->mainPage);

}

void mainWidget::on_pushButtonRegistrationMenu_clicked()
{
    ui->mainWidgets->setCurrentWidget(ui->Regist);
}

void mainWidget::on_pushButtonRegist_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString name = ui->nameLineEdit->text();
    QString email = ui->emailLineEdit->text();

    if (username.isEmpty() || password.isEmpty() || name.isEmpty() || email.isEmpty())
    {
        QMessageBox::information(nullptr, "", "Пожалуйста, заполните все поля.");
    }

    QSqlQuery checkQuery1;
    checkQuery1.prepare("SELECT COUNT(*) FROM Clients WHERE username = :username");
    checkQuery1.bindValue(":username", username);
    checkQuery1.exec();
    checkQuery1.next();

    if (checkQuery1.value(0).toInt() > 0) {
        QMessageBox::information(nullptr, "", "Пользователь с таким именем уже существует.");
    }

    QSqlQuery checkQuery2;
    checkQuery2.prepare("SELECT COUNT(*) FROM Clients WHERE email = :email");
    checkQuery2.bindValue(":email", email);
    checkQuery2.exec();
    checkQuery2.next();

    if (checkQuery2.value(0).toInt() > 0) {
        QMessageBox::information(nullptr, "", "Пользователь с такой почтой уже существует.");
    }

    QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query;
    query.prepare("INSERT INTO Clients (username, password, name, email) VALUES (:username, :password, :name, :email)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":name", name);
    query.bindValue(":email", email);

    if (query.exec())
    {
        QMessageBox::information(nullptr, "", "Регистрация прошла успешно!");
    } else {
        QMessageBox::information(nullptr, "", "Ошибка регистрации: " + query.lastError().text());
    }
    ui->mainWidgets->setCurrentWidget(ui->Authorization);
    if(!(ui->radioButtonEmployee->isChecked()))
    {
        ui->lineEditCode->setDisabled(true);
        ui->labelCode->setDisabled(true);
    }
}

void mainWidget::on_pushButtonBackMain_clicked()
{
    ui->mainWidgets->setCurrentWidget(ui->startPage);
}


void mainWidget::on_pushButtonAuthorization_clicked()
{
    ui->mainWidgets->setCurrentWidget(ui->Authorization);

    if(!(ui->radioButtonEmployee->isChecked()))
    {
        ui->lineEditCode->setDisabled(true);
        ui->labelCode->setDisabled(true);
    }
}


void mainWidget::on_radioButtonClient_clicked()
{
    ui->lineEditCode->setDisabled(true);
    ui->labelCode->setDisabled(true);
}

void mainWidget::on_radioButtonEmployee_clicked()
{
    ui->lineEditCode->setEnabled(true);
    ui->labelCode->setEnabled(true);
}


void mainWidget::on_pushButtonEntering_clicked()
{
    if(ui->radioButtonEmployee->isChecked())
    {
        QString username = ui->lineEdit->text();
        QString password = ui->lineEdit_2->text();
        QString code = ui->lineEditCode->text();

        if (username.isEmpty() || password.isEmpty() || code.isEmpty())
        {
            QMessageBox::information(nullptr, "", "Пожалуйста, заполните все поля.");
        }

        QString rightCode = getDay();

        QSqlQuery query;
        query.prepare("SELECT password FROM Staff WHERE username = :username");
        query.bindValue(":username", username);

        QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

        if (query.exec() && query.next()) {
            QString dbPassword = query.value(0).toString();
            if (dbPassword == hashedPassword && code == rightCode) {
                QMessageBox::information(this, "", "Вход выполнен успешно!");
            } else {
                QMessageBox::information(this, "", "Неверный логин или пароль или код!");
            }
        } else {
            QMessageBox::information(this, "", "Неверный логин или пароль или код!");
        }
    }
    else if(ui->radioButtonClient->isChecked())
    {
        QString username = ui->lineEdit->text();
        QString password = ui->lineEdit_2->text();

        if (username.isEmpty() || password.isEmpty())
        {
            QMessageBox::information(nullptr, "", "Пожалуйста, заполните все поля.");
        }

        QString rightCode = getDay();

        QSqlQuery query;
        query.prepare("SELECT password FROM Clients WHERE username = :username");
        query.bindValue(":username", username);

        QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

        if (query.exec() && query.next()) {
            QString dbPassword = query.value(0).toString();
            if (dbPassword == hashedPassword) {
                QMessageBox::information(this, "", "Вход выполнен успешно!");
            } else {
                QMessageBox::information(this, "", "Неверный логин или пароль!");
            }
        } else {
            QMessageBox::information(this, "", "Неверный логин или пароль!");
        }
    }
}


void mainWidget::on_pushButton_2_clicked()
{
    ui->mainWidgets->setCurrentWidget(ui->startPage);
}

