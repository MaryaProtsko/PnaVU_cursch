#include "MenuForm.h"
#include "ui_MenuForm.h"

MenuForm::MenuForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuForm)
{
    ui->setupUi(this);

    connect(ui->logOut, &QPushButton::clicked, this, &MenuForm::onLogOutButtonClicked);
}

MenuForm::~MenuForm()
{
    delete ui;
}

void MenuForm::onLogOutButtonClicked()
{
    emit logOut();
}
