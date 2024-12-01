#include "buttonhandler.h"
#include "ui_mainwindow.h"

ButtonHandler::ButtonHandler(MainWindow *mainWindow, QWidget *parent) : QObject(parent), mainWindow(mainWindow) {}

void ButtonHandler::createFileButtons(QVBoxLayout *layout) {
    QGroupBox *fileGroupBox = new QGroupBox("Файлы", mainWindow);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroupBox);
    QPushButton *loadGraphicsButton = new QPushButton("Загрузить графики", mainWindow);
    QPushButton *saveGraphicsButton = new QPushButton("Сохранить графики", mainWindow);
    fileLayout->addWidget(loadGraphicsButton);
    fileLayout->addWidget(saveGraphicsButton);
    layout->addWidget(fileGroupBox);

    connect(loadGraphicsButton, &QPushButton::clicked, mainWindow, &MainWindow::loadGraphics);
    connect(saveGraphicsButton, &QPushButton::clicked, mainWindow, &MainWindow::saveGraphics);
}

void ButtonHandler::createEditButtons(QVBoxLayout *layout) {
    QGroupBox *editGroupBox = new QGroupBox("Редактирование", mainWindow);
    QVBoxLayout *editLayout = new QVBoxLayout(editGroupBox);
    QPushButton *addLineEditButton = new QPushButton("Добавить график", mainWindow);
    QPushButton *clearAllButton = new QPushButton("Очистить всё", mainWindow);
    editLayout->addWidget(addLineEditButton);
    editLayout->addWidget(clearAllButton);
    layout->addWidget(editGroupBox);

    connect(addLineEditButton, &QPushButton::clicked, mainWindow, &MainWindow::addLineEditButtonClicked);
    connect(clearAllButton, &QPushButton::clicked, mainWindow, &MainWindow::clearAll);
}

void ButtonHandler::createSystemButtons(QVBoxLayout *layout) {
    QGroupBox *systemGroupBox = new QGroupBox("Система", mainWindow);
    QVBoxLayout *systemLayout = new QVBoxLayout(systemGroupBox);

    QPushButton *toggleSystemButton = new QPushButton("Сменить тип системы", mainWindow);
    QPushButton *functionsButton = new QPushButton("Шаблоны", mainWindow);
    QHBoxLayout *firstRowLayout = new QHBoxLayout();
    firstRowLayout->addWidget(toggleSystemButton);
    firstRowLayout->addWidget(functionsButton);

    QPushButton *multiplyByTwoButton = new QPushButton("x2", mainWindow);
    QPushButton *modTwoButton = new QPushButton("%2", mainWindow);
    QHBoxLayout *secondRowLayout = new QHBoxLayout();
    secondRowLayout->addWidget(multiplyByTwoButton);
    secondRowLayout->addWidget(modTwoButton);

    systemLayout->addLayout(firstRowLayout);
    systemLayout->addLayout(secondRowLayout);
    layout->addWidget(systemGroupBox);

    connect(toggleSystemButton, &QPushButton::clicked, mainWindow, &MainWindow::onToggleSystemButtonClicked);
    connect(functionsButton, &QPushButton::clicked, mainWindow, &MainWindow::openFunctionDialog);
    connect(multiplyByTwoButton, &QPushButton::clicked, mainWindow, &MainWindow::zoomInEvent);
    connect(modTwoButton, &QPushButton::clicked, mainWindow, &MainWindow::zoomOutEvent);
}


void ButtonHandler::addButtonsToLayout(const QStringList &buttons, QGridLayout *layout, QDialog *dialog) {
    int row = 0, col = 0;
    const int maxColumns = 6;
    for (const QString &buttonText : buttons) {
        QPushButton *button = new QPushButton(buttonText, dialog);
        layout->addWidget(button, row, col);
        connect(button, &QPushButton::clicked, mainWindow, [this, buttonText]() {
            mainWindow->addFunctionToLineEdit(buttonText);
        });
        col++;
        if (col == maxColumns) {
            col = 0;
            row++;
        }
    }
}
