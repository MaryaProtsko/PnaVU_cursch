//
// Created by Daniil on 28.10.2024.
//

#include "addDataWindow.h"

AddDataWindow::AddDataWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Добавить новый пароль");

    QLabel *nameLabel;
    nameLabel = new QLabel("Имя:", this);
    QLineEdit *nameEdit;
    nameEdit = new QLineEdit(this);
    nameEdit->setStyleSheet("QLineEdit {"
                            "border-radius: 10px;"
                            "padding: 5px;"
                            "}");

    QLabel *loginLabel;
    loginLabel = new QLabel("Логин:", this);
    QLineEdit *loginEdit;
    loginEdit = new QLineEdit(this);
    loginEdit->setStyleSheet("QLineEdit {"
                            "border-radius: 10px;"
                            "padding: 5px;"
                            "}");

    QLabel *passwordLabel;
    passwordLabel = new QLabel("Пароль:", this);
    QLineEdit *passwordEdit;
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("QLineEdit {"
                            "border-radius: 10px;"
                            "padding: 5px;"
                            "}");

    QDialogButtonBox *buttonBox;
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setStyleSheet("QPushButton {"
                             "border-radius: 15px;"  // Указываем радиус закругления
                             "background-color: #4CAF50;"  // Цвет фона
                             "color: white;"  // Цвет текста
                             "padding: 10px;"
                             "font-size: 10px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #266628;"  // Цвет фона при наведении
                             "color: gray;"
                             "}"
                             "QPushButton:pressed {"
                             "background-color: #388E3C;"
                             "}");

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddDataWindow::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &AddDataWindow::reject);

    QGridLayout *layout;
    layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameEdit, 0, 1);
    layout->addWidget(loginLabel, 1, 0);
    layout->addWidget(loginEdit, 1, 1);
    layout->addWidget(passwordLabel, 2, 0);
    layout->addWidget(passwordEdit, 2, 1);
    layout->addWidget(buttonBox, 3, 1);
    setLayout(layout);

    this->nameEdit = nameEdit;
    this->loginEdit = loginEdit;
    this->passwordEdit = passwordEdit;

}

QString AddDataWindow::getName() const {
    return nameEdit->text();
}

QString AddDataWindow::getLogin() const {
    return loginEdit->text();
}

QString AddDataWindow::getPassword() const {
    return passwordEdit->text();
}
