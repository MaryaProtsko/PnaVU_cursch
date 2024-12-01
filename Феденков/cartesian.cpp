#include"Cartesian.h"



std::vector<double>  DrawCartesian::getCurrentBorders(){
    std::vector<double> borders ={currentWidth,currentHeight};
    return borders;
};

void  DrawCartesian::setCurrentBorders(double newWidth,double newHeight){
    currentWidth = newWidth;
    currentHeight = newHeight;
};



void  DrawCartesian::setCartesian(QGraphicsScene * scene, double width, double height,int zoomMode,double dx,double dy){

    static double stepX = scene->width()/10;
    static double stepY = scene->height()/10;
    bool isEven = true;

    setCurrentBorders(width,height);
    drawAxis(scene,dx,dy);

    // при нулевом режиме зумирования мы просто должны имзенить значения подписей на осях и вернуться к классической сетке
    if(zoomMode == 0){
        stepX=scene->width()/10;
        stepY = scene->height()/10;
    }
    // режим увеличения масштаба
    else if(zoomMode == 1) {
        stepX=13*stepX/12;
        stepY=13*stepY/12;
    }
    // режим уменьшения масштаба
    else if(zoomMode == 2){
        stepX=12*stepX/13;
        stepY=12*stepY/13;
    }

    static_cast<int>(width/5) % 2 != 0 ? isEven=false: isEven=true;

    drawGrid(scene,stepX,stepY,isEven,dx,dy);

    divisionsDrawing(scene, stepX, stepY,dx,dy);

    nameDivisions(scene,stepX,stepY,dx,dy);

   };


void DrawCartesian::drawAxis(QGraphicsScene *scene, double dx, double dy) {
    static double xAxisHeight = scene->height() / 2; // Изначальное положение оси X
    static double yAxisWidth = scene->width() / 2; // Изначальное положение оси Y

    // Обновляем положение осей
    if (dy != 0) {
        xAxisHeight += dy;
    }
    if (dx != 0) {
        yAxisWidth += dx;
    }

    // Отрисовка осей
    scene->addLine(0, xAxisHeight, scene->width(), xAxisHeight); //penGreen);
    scene->addLine(yAxisWidth, 0, yAxisWidth, scene->height());// penGreen);
}

void DrawCartesian::drawGrid(QGraphicsScene *scene, double stepX, double stepY, bool isEven, double dx, double dy) {
    QPen penLightGray(Qt::lightGray, 0.5);
    QPen penGray(Qt::darkGray, 1);
    static QPointF center = scene->sceneRect().center(); // Сохраняем центр

    // Обновляем центр сетки
    center += QPointF(dx, dy);

    int cycleCounter = 0;
    int stepsPerStepAmount = isEven ? 4 : 5;

    // Рисуем линии по оси X
    for (double i = center.x() - stepX / stepsPerStepAmount; i > 0; i -= stepX / stepsPerStepAmount) {
        cycleCounter++;
        if (cycleCounter % stepsPerStepAmount == 0)
            scene->addLine(i, 0, i, scene->height(), penGray);
        else
            scene->addLine(i, 0, i, scene->height(), penLightGray);
    }

    cycleCounter = 0;
    for (double i = center.x() + stepX / stepsPerStepAmount; i < scene->width(); i += stepX / stepsPerStepAmount) {
        cycleCounter++;
        if (cycleCounter % stepsPerStepAmount == 0)
            scene->addLine(i, 0, i, scene->height(), penGray);
        else
            scene->addLine(i, 0, i, scene->height(), penLightGray);
    }

    cycleCounter = 0;

    // Рисуем линии по оси Y
    for (double i = center.y() - stepY / stepsPerStepAmount; i > 0; i -= stepY / stepsPerStepAmount) {
        cycleCounter++;
        if (cycleCounter % stepsPerStepAmount == 0)
            scene->addLine(0, i, scene->width(), i, penGray);
        else
            scene->addLine(0, i, scene->width(), i, penLightGray);
    }

    cycleCounter = 0;
    for (double i = center.y() + stepY / stepsPerStepAmount; i < scene->height(); i += stepY / stepsPerStepAmount) {
        cycleCounter++;
        if (cycleCounter % stepsPerStepAmount == 0)
            scene->addLine(0, i, scene->width(), i, penGray);
        else
            scene->addLine(0, i, scene->width(), i, penLightGray);
    }
}


void DrawCartesian::divisionsDrawing(QGraphicsScene *scene, double stepX, double stepY, double dx, double dy) {
    QPen penBlack(Qt::black, 1.5);
    static double xAxisHeight = scene->height() / 2; // Изначальное положение оси X
    static double yAxisWidth = scene->width() / 2;   // Изначальное положение оси Y

    // Обновляем положение осей
    if (dy != 0) {
        xAxisHeight += dy;
    }
    if (dx != 0) {
        yAxisWidth += dx;
    }

    // Отрисовка делений на оси OX
    for (double i = yAxisWidth - stepX; i > 0; i -= stepX) {
        scene->addLine(i, xAxisHeight + 5, i, xAxisHeight - 5, penBlack);
    }

    for (double i = yAxisWidth + stepX; i < scene->width(); i += stepX) {
        scene->addLine(i, xAxisHeight + 5, i, xAxisHeight - 5, penBlack);
    }

    // Отрисовка делений на оси OY
    for (double i = xAxisHeight - stepY; i > 0; i -= stepY) {
        scene->addLine(yAxisWidth + 5, i, yAxisWidth - 5, i, penBlack);
    }

    for (double i = xAxisHeight + stepY; i < scene->height(); i += stepY) {
        scene->addLine(yAxisWidth + 5, i, yAxisWidth - 5, i, penBlack);
    }
}


void DrawCartesian::nameDivisions(QGraphicsScene *scene, double stepX, double stepY, double dx, double dy) {
    static double xAxisHeight = scene->height() / 2; // Изначальное положение оси X
    static double yAxisWidth = scene->width() / 2;   // Изначальное положение оси Y

    // Обновляем положение осей
    if (dy != 0)
        xAxisHeight += dy;
    if (dx != 0)
        yAxisWidth += dx;

    // Подпись центра координат
    QGraphicsTextItem *centerTextItem = scene->addText(QString::number(0));
    centerTextItem->setPos(yAxisWidth - 10, xAxisHeight + 5);


    static double numOX = 0;
    static double numOY = 0;

    // Подписи делений на оси OX
    numOX = 0;
    for (double i = yAxisWidth - stepX; i > 0; i -= stepX) {
        numOX -= currentWidth / 5;
        QGraphicsTextItem *textItem = scene->addText(QString::number(numOX));
        textItem->setPos(i - 10, xAxisHeight + 5);
    }

    numOX = 0;
    for (double i = yAxisWidth + stepX; i < scene->width(); i += stepX) {
        numOX += currentWidth / 5;
        QGraphicsTextItem *textItem = scene->addText(QString::number(numOX));
        textItem->setPos(i - 10, xAxisHeight + 5);
    }

    // Подписи делений на оси OY
    numOY = 0;
    for (double i = xAxisHeight - stepY; i > 0; i -= stepY) {
        numOY += currentHeight / 5;
        QGraphicsTextItem *textItem = scene->addText(QString::number(numOY));
        textItem->setPos(yAxisWidth - 25, i - 10);
    }

    numOY = 0;
    for (double i = xAxisHeight + stepY; i < scene->height(); i += stepY) {
        numOY -= currentHeight / 5;
        QGraphicsTextItem *textItem = scene->addText(QString::number(numOY));
        textItem->setPos(yAxisWidth - 25, i - 10);
    }
}
