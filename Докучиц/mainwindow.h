#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow; // основной класс отображения окна
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Log_triggered(); // функция обработки кнопки верхней строки меню для входа

    void on_Reg_triggered(); // функция обработки кнопки верхней строки меню для регистарции

    void on_registration_back_clicked(); // кнопка возвращение на главный экран

    void on_login_back_clicked(); // кнопка возвращение на главный экран

    void on_pushButton_login_2_clicked();

    void on_pushButton_login_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
