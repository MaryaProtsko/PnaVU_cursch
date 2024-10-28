//
// Created by Daniil on 28.10.2024.
//

#include "addDataWindow.h"

AddDataWindow::AddDataWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Добавить новый пароль");
    QLabel *label;
    label = new QLabel("Это новое окно", this);
    QVBoxLayout *layout;
    layout = new QVBoxLayout(this);
    layout->addWidget(label);
    setLayout(layout);
    resize(200, 100);
}
