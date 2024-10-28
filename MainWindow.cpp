//
// Created by Daniil on 28.10.2024.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget *widget;
    widget = new QWidget(this);
    setCentralWidget(widget);

    QPushButton *addButton;
    addButton = new Button("Добавить данные", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::openAddDataWindow);

    QPushButton *viewButton;
    viewButton = new Button("Посмотреть данные", this);

    QVBoxLayout *layout;
    layout = new QVBoxLayout(widget);
    layout->addWidget(addButton);
    layout->addWidget(viewButton);

    setWindowTitle("Qt Project");
    resize(400, 300);
}

void MainWindow::openAddDataWindow() {
    AddDataWindow addDataWindow(this);
    addDataWindow.exec();
}
