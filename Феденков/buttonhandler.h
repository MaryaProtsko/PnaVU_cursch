#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <QObject>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialog>
#include <QWidget>
#include <QStringList>
#include "mainwindow.h"

class ButtonHandler : public QObject {
    Q_OBJECT

public:
    explicit ButtonHandler(MainWindow *mainWindow, QWidget *parent = nullptr);
    void createFileButtons(QVBoxLayout *layout);
    void createEditButtons(QVBoxLayout *layout);
    void createSystemButtons(QVBoxLayout *layout);
    void openFunctionDialog();

private:
    MainWindow *mainWindow;
    void addButtonsToLayout(const QStringList &buttons, QGridLayout *layout, QDialog *dialog);
};

#endif // BUTTONHANDLER_H
