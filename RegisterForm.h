#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = nullptr);
    ~RegisterForm();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::RegisterForm *ui;

    void setStyles();
    void clearData();
signals:
    void registerSuccess();
    void back();

private slots:
    void onRegisterButtonClicked();
    void onBackButtonClicked();
};

#endif // REGISTERFORM_H
