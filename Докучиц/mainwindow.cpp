/* в данном файле идет отображение самого интерфейса
 Что бы меньше разбератся раскажу что есть:
1 - Регистарция пользователя в базу данных (если зарегестрирова и так далее не верные парва то вывод ошибки)
2 - Вход пользователя с обработками ошибок
3- Считывание прав доступа и после этого преход на нужное окно для админа и обычного юсера с разным интерфейсом для каждого из них
 Реалиацию окон сделал через StackWidget
*/
#include "mainwindow.h"
#include "AccessCodeDialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->Default);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Log_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->Login); // принажатии на кнопку в меню сверху измение на виджет входа
}


void MainWindow::on_Reg_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->Registration);
}


void MainWindow::on_registration_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Default);
}


void MainWindow::on_login_back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Default);
}

void MainWindow::on_pushButton_login_2_clicked()
{
    QString username = ui->lineEdit_login_2->text();
    QString password = ui->lineEdit_password_2->text();
    QString role = ui->comboBox->currentText();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM User WHERE username = :username");
    checkQuery.bindValue(":username", username);
    checkQuery.exec();
    checkQuery.next();

    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Предупреждение", "Такой пользователь уже существует.");
        return;
    }

    if (role == "товарищ Майор") {
        AccessCodeDialog accessCodeDialog(this);
        if (!accessCodeDialog.exec() || !accessCodeDialog.isAccessCodeValid()) {
            return;  // Если код неверный или диалог отменён, регистрация прерывается
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO User (username, password, role) VALUES (:username, :password, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (role == "товарищ Майор")
    {
        query.bindValue(":role", "admin");
    }
    else
    {
        query.bindValue(":role", "user");
    }


    if (query.exec()) {

        if (role == "товарищ Майор") {
            QMessageBox::information(this, "Приветствие", "Здравия желаем " + role + "!!!");
        }
        else
        {
            QMessageBox::information(this, "Приветствие", "Здравствуйте! Регистрация прошла успешно");
        }
    } else {

    }
}

void MainWindow::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_login->text();
    QString password = ui->lineEdit_password->text();

    QSqlQuery query;
    query.prepare("SELECT role FROM User WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        QString role = query.value(0).toString();
        QMessageBox::information(this, "Авторизация", "Авторизация прошла успешно, дabро пожаловать " + role);
        // Здесь можно закрыть диалог и продолжить с логикой для роли
    } else {
        QMessageBox::warning(this, "Авторизация", "Неверный пороль или логин!");
    }
}

