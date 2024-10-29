/*
 Файл для окна уведомления при обработке кнопки добавления пользователя
 Если пользовтель хочет получить право доступа admin то ему нужно ввести специальный код пока что это строка

 */
#include "AccessCodeDialog.h"
#include "ui_AccessCodeDialog.h"
#include <QMessageBox>

AccessCodeDialog::AccessCodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccessCodeDialog),
    accessCodeValid(false) {
    ui->setupUi(this);

    // Соединяем кнопку подтверждения с проверкой кода
    connect(ui->confirmButton, &QPushButton::clicked, this, &AccessCodeDialog::onConfirmClicked);
}

AccessCodeDialog::~AccessCodeDialog() {
    delete ui;
}

void AccessCodeDialog::onConfirmClicked() {
    QString accessCode = ui->accessCodeLineEdit->text();

    // Проверка правильного кода
    if (accessCode == "82931") {
        accessCodeValid = true;
        accept();  // Закрываем диалог и возвращаем результат "принято"
    } else {
        accessCodeValid = false;
        QMessageBox::warning(this, "Ошибка", "Неверный код доступа для администратора." + accessCode);

        // Очищаем поле, если код неверный
        if (ui->confirmButton) {
            ui->accessCodeLineEdit->clear();
        }
    }
}

bool AccessCodeDialog::isAccessCodeValid() const {
    return accessCodeValid;
}
