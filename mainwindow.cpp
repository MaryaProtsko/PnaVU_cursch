#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gamefield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("Добро пожаловать!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}


void MainWindow::on_pushButton_released()
{
    ui->statusbar->showMessage(":)");
}


void MainWindow::on_pushButton_3_clicked()
{
    GameField window;
    window.setWindowTitle("Cудоку");
    window.setModal(true);
    window.exec();
}


void MainWindow::on_pushButton_pressed()
{
    ui->statusbar->showMessage(":(");
}

