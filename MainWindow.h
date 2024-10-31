#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "LoginForm.h"
#include "RegisterForm.h"
#include "MenuForm.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginForm *loginForm;
    RegisterForm *registerForm;
    MenuForm *menuForm;

    void paintEvent(QPaintEvent* event);

private slots:
    void showLoginForm();
    void showRegisterForm();
    void showMenuForm();
};
#endif // MAINWINDOW_H
