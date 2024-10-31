#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::LoginForm *ui;

    void setStyles();
    void clearData();

signals:
    void loginSuccess();
    void toRegisterForm();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
};

#endif // LOGINFORM_H
