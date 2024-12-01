/*#include"DrawPolar.h"

std::vector<double>  DrawPolar::getCurrentBorders(){
    std::vector<double> borders ={currentWidth,currentHeight};
    return borders;
};

void  DrawPolar::setCurrentBorders(double newWidth,double newHeight){
    currentWidth = newWidth;
    currentHeight = newHeight;
};


void  DrawPolar::setPolar(QGraphicsScene * scene, double width, double height,int zoomMode,double dx,double dy){

    static double stepX = scene->width()/10;
    static double stepY = scene->height()/10;
    bool isEven = true;

    setCurrentBorders(width,height);
    drawAxis(scene,dx,dy);

    // при нулевом режиме зумирования мы просто должны имзенить значения подписей на осях и вернуться к классической сетке
    if(zoomMode == 0){
        stepX = scene->width()/10;
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

    static_cast<int>(width/5) % 2 != 0 ? isEven = false: isEven=true;

    drawGrid(scene,stepX,stepY,isEven,dx,dy);

    nameDivisions(scene,stepX,stepY,dx,dy);

   };


void DrawPolar::drawAxis(QGraphicsScene *scene, double dx, double dy) {
    static double xAxisHeight = scene->height() / 2; // Изначальное положение оси X
    static double yAxisWidth = scene->width() / 2;   // Изначальное положение оси Y

    // Обновляем положение осей при смещении
    xAxisHeight += dy;
    yAxisWidth += dx;

    // Отрисовка осей
    scene->addLine(0, xAxisHeight, scene->width(), xAxisHeight);
    scene->addLine(yAxisWidth, 0, yAxisWidth, scene->height());
}

void DrawPolar::drawGrid(QGraphicsScene *scene, double stepX, double stepY, bool isEven, double dx, double dy) {
    QPen penLightGray(Qt::lightGray, 0.5);
    QPen penGray(Qt::darkGray, 1);
    static QPointF center = scene->sceneRect().center(); // Сохраняем центр

       // Обновляем центр сетки
       center += QPointF(dx, dy);

    int cycleCounter = 0;
    int stepsPerStepAmount = isEven ? 4 : 5;

        // Рисование окружностей до текущих границ сцены
        //double maxRadius = std::hypot(scene->width(), scene->height());
        for (double i = stepX/stepsPerStepAmount; i < scene->width(); i += stepX/stepsPerStepAmount) {
            QRect rect(center.x() - i, center.y() - i, 2 * i, 2 * i);
            cycleCounter++;

            if (cycleCounter % stepsPerStepAmount == 0) {
                scene->addEllipse(rect, penGray);
                if (cycleCounter == 3 * stepsPerStepAmount) {
                    addAngleLabels(scene, center, i, dx, dy);
                }
            } else
                scene->addEllipse(rect, penLightGray);
        }


    // Рисование направляющих линий (лучей) из центра
    QPointF endPoint(scene->width(), center.y());
    int angleStep = 15; // шаг угла

    for (int angle = 0; angle < 360; angle += angleStep) {
        QTransform rotationMatrix;
        rotationMatrix.rotate(angle);
        QPointF rotatedEndPoint = rotationMatrix.map(endPoint - center) + center;

        QLineF line(center, rotatedEndPoint);
        scene->addLine(line, penLightGray);
    }
}

// Функция для добавления подписей углов на третьем круге
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

void DrawPolar::nameDivisions(QGraphicsScene *scene, double stepX, double stepY, double dx, double dy) {
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
    for (double i = xAxisHeight - stepX; i > 0; i -= stepX) {
        numOY += currentHeight / 5;
        QGraphicsTextItem *textItem = scene->addText(QString::number(numOY));
        textItem->setPos(yAxisWidth-25, i-10);
    }

    numOY = 0;
    for (double i = xAxisHeight + stepX; i < scene->height(); i += stepX) {
        numOY -= currentHeight / 5;
        QGraphicsTextItem *textItem = scene->addText(QString::number(numOY));
        textItem->setPos(yAxisWidth - 25, i - 10);
    }
}

*/
