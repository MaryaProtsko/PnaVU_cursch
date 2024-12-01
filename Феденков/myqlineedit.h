#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class MyLineEdit : public QLineEdit {
    Q_OBJECT

public:
    explicit MyLineEdit(QWidget *parent = nullptr);

protected:
    void focusInEvent(QFocusEvent *event) override;

signals:
    void focused(); // Сигнал, который будет испускаться при получении фокуса
};

#endif // MYLINEEDIT_H
