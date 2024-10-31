#include <QFontDatabase>
#include <QStyleOption>
#include <QPainter>
#include <sqlite3.h>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Database.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    Database::initDatabase();

    ui->setupUi(this);

    setStyleSheet(R"(
        QWidget#MainWindow {
            background-color: #e6e6fa;
        }
    )");

    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono.ttf");

    loginForm = new LoginForm();
    registerForm = new RegisterForm();
    menuForm = new MenuForm();

    ui->stackedWidget->addWidget(loginForm);
    ui->stackedWidget->addWidget(registerForm);
    ui->stackedWidget->addWidget(menuForm);

    connect(loginForm, &LoginForm::loginSuccess, this, &MainWindow::showMenuForm);
    connect(loginForm, &LoginForm::toRegisterForm, this, &MainWindow::showRegisterForm);
    connect(registerForm, &RegisterForm::registerSuccess, this, &MainWindow::showLoginForm);
    connect(registerForm, &RegisterForm::back, this, &MainWindow::showLoginForm);
    connect(menuForm, &MenuForm::logOut, this, &MainWindow::showLoginForm);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showRegisterForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showMenuForm()
{
    ui->stackedWidget->setCurrentIndex(2);
}
