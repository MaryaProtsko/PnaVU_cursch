#include "myqlineedit.h"

MyLineEdit::MyLineEdit(QWidget *parent)
: QLineEdit(parent) {}

void MyLineEdit::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);
    emit focused(); // emits signal
}
