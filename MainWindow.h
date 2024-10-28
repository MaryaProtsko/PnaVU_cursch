//
// Created by Daniil on 28.10.2024.
//

#ifndef QTPROJECT_MAINWINDOW_H
#define QTPROJECT_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "Button.h"
#include "addDataWindow.h"


class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private slots:
    void openAddDataWindow();
};


#endif //QTPROJECT_MAINWINDOW_H
