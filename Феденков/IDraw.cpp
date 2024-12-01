#include "IDraw.h"

double IDraw::dx = 0.0;
double IDraw::dy = 0.0;

std::vector<double>IDraw::getCurrentBorders(){
    std::vector<double> borders = { currentWidth,currentHeight };
    return borders;
}

void IDraw::setCurrentBorders(double newWidth, double newHeight){
    currentWidth = newWidth;
    currentHeight = newHeight;
}

void IDraw::setSystem(QGraphicsScene* scene, double height, double width, int zoomMode,bool wasDrugged, double dx, double dy){
    center = scene->sceneRect().center() + offset;
    static double stepX = 10;
    static double stepY = 10;
    bool isEven = true;

    setCurrentBorders(width, height);
    drawAxis(scene);
    if(wasDrugged==false){
    // при нулевом режиме зумирования мы просто должны имзенить значения подписей на осях и вернуться к классической сетке
    if (zoomMode == 0) {
        stepX = scene->width() / 10;
        stepY = scene->width() / 10;
    }
    // режим увеличения масштаба
    else if (zoomMode == 1) {
        stepX = 13 * stepX / 12;
        stepY = 13 * stepY / 12;
    }
    // режим уменьшения масштаба
    else if (zoomMode == 2) {
        stepX = 12 * stepX / 13;
        stepY = 12 * stepY / 13;
    }
    }

    static_cast<int>(width / 5) % 2 != 0 ? isEven = false : isEven = true;

    drawGrid(scene, stepX, stepY, isEven, dx, dy);

    divisionsDrawing(scene, stepX, stepY);

    nameDivisions(scene, stepX);


}

void IDraw::drawAxis(QGraphicsScene* scene){
     double xAxisHeight = center.y(); // Изначальное положение оси X
     double yAxisWidth = center.x();  // Изначальное положение оси Y

    // Отрисовка осей
    scene->addLine(0, xAxisHeight, scene->width(), xAxisHeight); //penGreen);
    scene->addLine(yAxisWidth, 0, yAxisWidth, scene->height());// penGreen);
}

void IDraw::nameDivisions(QGraphicsScene* scene,double stepX){
   double xAxisHeight = center.y(); // Изначальное положение оси X
   double yAxisWidth = center.x();  // Изначальное положение оси Y

    // Подпись центра координат
    QGraphicsTextItem* centerTextItem = scene->addText(QString::number(0));
    centerTextItem->setPos(yAxisWidth - 10, xAxisHeight + 5);


    static double numOX = 0;
    static double numOY = 0;

    // Подписи делений на оси OX
    numOX = 0;
    for (double i = yAxisWidth - stepX; i > 0; i -= stepX) {
        numOX -= currentWidth / 5;
        QGraphicsTextItem* textItem = scene->addText(QString::number(numOX));
        textItem->setPos(i - 10, xAxisHeight + 5);
    }

    numOX = 0;
    for (double i = yAxisWidth + stepX; i < scene->width(); i += stepX) {
        numOX += currentWidth / 5;
        QGraphicsTextItem* textItem = scene->addText(QString::number(numOX));
        textItem->setPos(i - 10, xAxisHeight + 5);
    }

    // Подписи делений на оси OY
    numOY = 0;
    for (double i = xAxisHeight - stepX; i > 0; i -= stepX) {
        numOY += currentHeight / 5;
        QGraphicsTextItem* textItem = scene->addText(QString::number(numOY));
        textItem->setPos(yAxisWidth - 25, i - 10);
    }

    numOY = 0;
    for (double i = xAxisHeight + stepX; i < scene->height(); i += stepX) {
        numOY -= currentHeight / 5;
        QGraphicsTextItem* textItem = scene->addText(QString::number(numOY));
        textItem->setPos(yAxisWidth - 25, i - 10);
    }
}


void DrawCartesian::drawGrid(QGraphicsScene* scene,double stepX, double stepY, bool isEven,double dx,double dy) {
    QPen penLightGray(Qt::lightGray, 0.5);
    QPen penGray(Qt::darkGray, 1);

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
void DrawCartesian::divisionsDrawing(QGraphicsScene* scene,double stepX, double stepY) {
    QPen penBlack(Qt::black, 1.5);
    double xAxisHeight = center.y();
    double yAxisWidth = center.x();

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

void DrawPolar::drawGrid(QGraphicsScene* scene,double stepX, double stepY, bool isEven,double dx,double dy) {
    QPen penLightGray(Qt::lightGray, 0.5);
    QPen penGray(Qt::darkGray, 1);

    int cycleCounter = 0;
    int stepsPerStepAmount = isEven ? 4 : 5;

    // Рисование окружностей до текущих границ сцены
    static double maxRadius = scene->width();
    if(dx)
        maxRadius+=abs(dx);
    for (double i = stepX / stepsPerStepAmount; i < maxRadius; i += stepX / stepsPerStepAmount) {
        QRect rect(center.x() - i, center.y() - i, 2 * i, 2 * i);
        cycleCounter++;

        if (cycleCounter % stepsPerStepAmount == 0) {
           scene->addEllipse(rect, penGray);
            if (cycleCounter == 3 * stepsPerStepAmount) {
                addAngleLabels(scene, center, i, dx, dy);
            }
        }
        else
            scene->addEllipse(rect, penLightGray);
    }


    // Рисование направляющих линий (лучей) из центра
    QPointF universalCenter(scene->width() / 2, scene->height() / 2);
       double maxDimension = std::hypot(scene->width(), scene->height())+std::hypot(center.x(),center.y()); // Максимальная длина линии
       int angleStep = 15; // шаг угла

       for (int angle = 0; angle < 360; angle += angleStep) {
           QTransform rotationMatrix;
           rotationMatrix.rotate(angle);
           QPointF rotatedEndPoint = rotationMatrix.map(QPointF(maxDimension, 0)) + center;

           QLineF line(center, rotatedEndPoint);
           scene->addLine(line, penLightGray);
       }
   }

void DrawPolar::divisionsDrawing(QGraphicsScene* scene,double stepX, double stepY) {
    return;
}

void DrawPolar::addAngleLabels(QGraphicsScene *scene, const QPointF &center, double radius, double dx, double dy) {
    int angleStep = 15;
    for (int angle = 0; angle < 360; angle += angleStep) {
        double rad = qDegreesToRadians((double)angle);
        QPointF labelPos(center.x() + radius * cos(rad), center.y() - radius * sin(rad));

        // Добавляем смещение
        labelPos += QPointF(dx, dy);

        // Создаём и позиционируем подпись угла
        QGraphicsTextItem *angleLabel = scene->addText(QString::number(angle) + "°");
        angleLabel->setPos(labelPos.x() - 10, labelPos.y() - 10);
    }
}
