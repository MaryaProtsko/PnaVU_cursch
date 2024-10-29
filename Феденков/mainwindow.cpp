#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Function.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , cartesian(new DrawCartesian())
    , view(new CustomGraphicsView(scene, cartesian, this))
    , mainLayout(new QHBoxLayout())
    , userInputLayout(new QVBoxLayout()) // Вертикальное расположение QLineEdit
    , inputPanel(new QWidget(this)) // Панель для ввода
    , inputCount(0) // Счетчик QLineEdit
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

    addLineEdit();
}

void MainWindow::addLineEdit() {
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    userInputLayout->addWidget(lineEdit);
    inputCount++; // Увеличиваем счетчик
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (inputCount < 25){
            drawGraphics();
            addLineEdit();
        }
    }
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
void MainWindow::setView(QGraphicsView *viewToSet){
    view=viewToSet;
}



void MainWindow::resizeEvent(QResizeEvent *event) {
   static bool i=false;
    if(i){
        QMainWindow::resizeEvent(event);
        QSize viewSize = view->size();
        scene->setSceneRect(0, 0, viewSize.width(), viewSize.height());
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        scene->clear();
        cartesian->setCartesian(scene, cartesian->currentHeight, cartesian->currentWidth, 0, 0, 0);

       // if (proxy)
       //     proxy->setPos(scene->width() - button->width() - 10, 10);
    }
    i=true;
}


/*void MainWindow::increaseScaleByButton(){
}*/



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::drawGraphics() {
    // Получаем последний добавленный QLineEdit
    QLineEdit *lastLineEdit = qobject_cast<QLineEdit*>(userInputLayout->itemAt(inputCount - 1)->widget());

    if (lastLineEdit) {
        // Получаем текст, введенный пользователем, и преобразуем его в std::string
        QString inputText = lastLineEdit->text();
        std::string userInput = inputText.toStdString();

        // Проверяем, что текст не пустой и заканчивается символом ';'
        if (!userInput.empty()) {
            Function function(userInput);

            for(double i=-cartesian->currentWidth;i<=cartesian->currentWidth;i+=1){
                function.setX(i);
                double meh = function.evaluate();
            }
        }
    }
    return;
}
