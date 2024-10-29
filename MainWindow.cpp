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
    connect(viewButton, &QPushButton::clicked, this, &MainWindow::showData);

    QVBoxLayout *layout;
    layout = new QVBoxLayout(widget);
    layout->addWidget(addButton);
    layout->addWidget(viewButton);

    setWindowTitle("Qt Project");
    resize(400, 300);
}

void MainWindow::openAddDataWindow() {
    AddDataWindow addDataWindow(this);
    if(addDataWindow.exec() == QDialog::Accepted) {
        QString name = addDataWindow.getName();
        QString login = addDataWindow.getLogin();
        QString password = addDataWindow.getPassword();

        saveToFile(name, login, password);
    }
}

void MainWindow::saveToFile(const QString &name, const QString &login, const QString &password) {
    QFile file("data.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << ' ' << login << ' ' << password << '\n';
        file.close();
        qDebug() << "Data safe in file";
    }
}

void MainWindow::showData() {
    QFile file("data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString data = in.readAll();
        file.close();

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Safe Data");
        msgBox.setText(data);

        msgBox.setCursor(Qt::PointingHandCursor);
        msgBox.setStyleSheet("QPushButton {"
                             "border-radius: 15px;"  // Указываем радиус закругления
                             "background-color: #4CAF50;"  // Цвет фона
                             "color: white;"  // Цвет текста
                             "padding: 10px;"
                             "font-size: 16px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #266628;"  // Цвет фона при наведении
                             "color: gray;"
                             "}"
                             "QPushButton:pressed {"
                             "background-color: #388E3C;"
                             "}");
        msgBox.exec();

    }
}
