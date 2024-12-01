
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Function.h"
#include"buttonhandler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , system(new DrawCartesian())
    , drawer(new GraphicDrawer())
    , view(new CustomGraphicsView(scene, system, drawer, &lineEditVector, this))
    , mainLayout(new QHBoxLayout())
    , userInputLayout(new QVBoxLayout())
    , inputPanel(new QWidget(this))
    , fileManager(new FileManager(scene, &lineEditVector, userInputLayout))
    , inputCount(0)
    , pointsCache()
{
    ui->setupUi(this);

    scene->setSceneRect(0, 0, this->width(), this->height());
    view->setRenderHint(QPainter::Antialiasing, false);
    view->setMouseTracking(true);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    inputPanel->setLayout(userInputLayout);
    mainLayout->addWidget(inputPanel, 1);
    mainLayout->addWidget(view, 3);
    centralWidget->setLayout(mainLayout);

    ButtonHandler *buttonHandler = new ButtonHandler(this);

    // Create buttons using the ButtonHandler
    buttonHandler->createFileButtons(userInputLayout);
    buttonHandler->createEditButtons(userInputLayout);
    buttonHandler->createSystemButtons(userInputLayout);
    addNewLineEdit();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete system;
    delete drawer;
    delete view;
    delete mainLayout;
    delete userInputLayout;
    delete inputPanel;
    delete fileManager;
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            drawGraphics();
}

QGraphicsScene* MainWindow::getScene()const{
    return scene;
}
void MainWindow::setScene(QGraphicsScene *sceneToSet){
    scene=sceneToSet;
}


QGraphicsView* MainWindow::getView() const{
    return view;
}



void MainWindow::resizeEvent(QResizeEvent *event) {
   static bool i=false;
    if(i){
        QMainWindow::resizeEvent(event);
        QSize viewSize = view->size();
        scene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        scene->clear();
        system->setSystem(scene, system->currentHeight, system->currentWidth, 0,false, system->dx, system->dy);
    }
    i=true;
}



void  MainWindow::zoomInEvent(){
    int zoomScaleFactor=2;
    scene->clear();
    system->setSystem(scene,system->getCurrentBorders()[0] / zoomScaleFactor, system->getCurrentBorders()[1] / zoomScaleFactor, 0,false, system->dx, system->dy);
    pointsCache = drawer->drawGraphics(scene, &lineEditVector, system,pointsCache,0,true,true);
}
void  MainWindow::zoomOutEvent(){
    int zoomScaleFactor=2;
    scene->clear();
    system->setSystem(scene,system->getCurrentBorders()[0] * zoomScaleFactor, system->getCurrentBorders()[1] * zoomScaleFactor, 0,false, system->dx, system->dy);
    pointsCache = drawer->drawGraphics(scene, &lineEditVector, system,pointsCache,0,true,true);
}


void MainWindow::drawGraphics() {
    lineEditVector.clear(); // Очищаем перед заполнением
    scene->clear();
    system->setSystem(scene,system->getCurrentBorders()[0], system->getCurrentBorders()[1], 0,false, system->dx, system->dy);
    for (int i = 0; i < userInputLayout->count(); i++) {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(userInputLayout->itemAt(i)->widget());
        if (lineEdit) {
            lineEditVector.push_back(lineEdit); // Добавляем указатель
        }
    }
    view->setLineEditVector(&lineEditVector);
   pointsCache = drawer->drawGraphics(scene, &lineEditVector, system,pointsCache,view->getZoomMode(),view->getWasDrugged());
   view->setPointsCache(pointsCache);
}


void MainWindow::onToggleSystemButtonClicked() {
    scene->clear();
QPointF offset=system->getOffset();
    // Переключение типа системы
    if (dynamic_cast<DrawCartesian*>(system)) {
        delete system;
        system = new DrawPolar();
    } else {
        delete system;
        system = new DrawCartesian();
    }
    // Обновление сцены с новой системой
    delete view;
    view = new CustomGraphicsView(scene, system,drawer,&lineEditVector, this);
    mainLayout->addWidget(view, 3);
    system->applyOffset(offset.x(),offset.y());
    system->setSystem(scene, system->currentHeight, system->currentWidth, 0,false,0,0);


}


void MainWindow::openFunctionDialog() {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Шаблоны");
    dialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint| Qt::WindowMinimizeButtonHint);
    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);

    // Группа "Переменные"
    QGroupBox *variablesGroup = new QGroupBox("Переменные");
    QGridLayout *variablesLayout = new QGridLayout();
    QStringList variables = {"x", "y", "theta", "r", "e", "pi"};
    addButtonsToLayout(variables, variablesLayout, dialog);
    variablesGroup->setLayout(variablesLayout);
    dialogLayout->addWidget(variablesGroup);

    // Группа "Операторы"
    QGroupBox *operatorsGroup = new QGroupBox("Операторы");
    QGridLayout *operatorsLayout = new QGridLayout();
    QStringList operators = {"=", "<=", ">=", "<", ">", "+", "-", "*", "/", "^2", "^3", "^"};
    addButtonsToLayout(operators, operatorsLayout, dialog);
    operatorsGroup->setLayout(operatorsLayout);
    dialogLayout->addWidget(operatorsGroup);

    // Группа "Функции"
    QGroupBox *functionsGroup = new QGroupBox("Функции");
    QGridLayout *functionsLayout = new QGridLayout();
    QStringList functions = {"sin()", "cos()", "tan()", "cot()", "arcsin()", "arccos()",
                             "arctan()", "arccot()", "log()", "lg()", "ln()", "root()",
                             "sqrt()", "abs()", "(", ")", ",", "."};
    addButtonsToLayout(functions, functionsLayout, dialog);
    functionsGroup->setLayout(functionsLayout);
    dialogLayout->addWidget(functionsGroup);

    // Группа "Цифры"
    QGroupBox *digitsGroup = new QGroupBox("Цифры");
    QGridLayout *digitsLayout = new QGridLayout();
    QStringList digits;
    for (int i = 0; i <= 9; ++i) {
        digits.append(QString::number(i));
    }
    addButtonsToLayout(digits, digitsLayout, dialog);
    digitsGroup->setLayout(digitsLayout);
    dialogLayout->addWidget(digitsGroup);

    // Группа "Управление курсором"
    QGroupBox *controlGroup = new QGroupBox("Управление курсором");
    QGridLayout *controlLayout = new QGridLayout();
    QPushButton *leftArrowButton = new QPushButton("←", dialog);
    QPushButton *rightArrowButton = new QPushButton("→", dialog);
    QPushButton *deleteButton = new QPushButton("Delete", dialog);
    QPushButton *calculateButton = new QPushButton("Рассчитать", dialog);
    controlLayout->addWidget(leftArrowButton, 0, 0);
    controlLayout->addWidget(rightArrowButton, 0, 1);
    controlLayout->addWidget(deleteButton, 1, 0);
    controlLayout->addWidget(calculateButton, 1, 1);
    controlGroup->setLayout(controlLayout);
    dialogLayout->addWidget(controlGroup);

    // Подключение сигналов для специальных кнопок
    connect(leftArrowButton, &QPushButton::clicked, this, [this]() {
        moveCursorLeft();
    });
    connect(rightArrowButton, &QPushButton::clicked, this, [this]() {
        moveCursorRight();
    });
    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        deleteLastCharacter();
    });
    connect(calculateButton, &QPushButton::clicked, this, [this]() {
        drawGraphics();
    });


    dialog->setLayout(dialogLayout);
    dialog->show();
}

void MainWindow::addButtonsToLayout(const QStringList &buttons, QGridLayout *layout, QDialog *dialog) {
    int row = 0, col = 0;
    const int maxColumns = 6;
    for (const QString &buttonText : buttons) {
        QPushButton *button = new QPushButton(buttonText, dialog);
        layout->addWidget(button, row, col);
        connect(button, &QPushButton::clicked, this, [this, buttonText]() {
            addFunctionToLineEdit(buttonText);
        });
        col++;
        if (col == maxColumns) {
            col = 0;
            row++;
        }
    }
}

void MainWindow::moveCursorLeft() {
    if (lastActiveLineEdit) {
        int cursorPos = lastActiveLineEdit->cursorPosition();
        lastActiveLineEdit->setCursorPosition(qMax(0, cursorPos - 1));
    }
}

void MainWindow::moveCursorRight() {
    if (lastActiveLineEdit) {
        int cursorPos = lastActiveLineEdit->cursorPosition();
        lastActiveLineEdit->setCursorPosition(qMin(lastActiveLineEdit->text().length(), cursorPos + 1));
    }
}

void MainWindow::deleteLastCharacter() {
    if (lastActiveLineEdit) {
        QString text = lastActiveLineEdit->text();
        int cursorPos = lastActiveLineEdit->cursorPosition();
        if (cursorPos > 0) {
            text.remove(cursorPos - 1, 1);
            lastActiveLineEdit->setText(text);
            lastActiveLineEdit->setCursorPosition(cursorPos - 1);
        }
    }
}

void MainWindow::addLineEditButtonClicked(){
    addNewLineEdit();
}

void MainWindow::addFunctionToLineEdit(const QString &text) {
    if (lastActiveLineEdit) {
        int cursorPos = lastActiveLineEdit->cursorPosition();
        QString currentText = lastActiveLineEdit->text();
        lastActiveLineEdit->setText(currentText.insert(cursorPos, text));
        lastActiveLineEdit->setCursorPosition(cursorPos + text.length());
    }
}

void MainWindow::onLineEditFocused() {
    MyLineEdit *focusedLineEdit = qobject_cast<MyLineEdit *>(sender());
    if (focusedLineEdit) {
        lastActiveLineEdit = focusedLineEdit;
    }
}

void MainWindow::addNewLineEdit() {
    MyLineEdit *newLineEdit = new MyLineEdit(this);
    userInputLayout->addWidget(newLineEdit);

    // Подключаем сигнал `focused` к слоту обновления активного поля
    connect(newLineEdit, &MyLineEdit::focused, this, &MainWindow::onLineEditFocused);
}


void MainWindow::clearAll() {
    for (QLineEdit* lineEdit : lineEditVector)
           lineEdit->clear();
    while (!lineEditVector.empty())
        lineEditVector.pop_back();
    if(pointsCache)
        pointsCache->clear();
    scene->clear();
    system->setSystem(scene,system->getCurrentBorders()[0], system->getCurrentBorders()[1], 0,false, system->dx, system->dy);
}

void MainWindow::saveGraphics() {
fileManager->saveGraphics();
}

void MainWindow::loadGraphics() {
    fileManager->loadGraphics();
}

