#include "GraphicDrawer.h"
#include<QVBoxLayout>
#include<QDialog>
#include<QLabel>
#include<QDoubleSpinBox>
#include<QPushButton>
#include <string>
#include <cmath>
#include <stdexcept>
#include <regex>


std::vector<QColor> colors = {
    QColor(255, 0, 0),    // красный
    QColor(0, 255, 0),    // зеленый
    QColor(0, 0, 255),    // синий
    QColor(255, 255, 0),  // желтый
    QColor(255, 0, 255),  // пурпурный
    QColor(0, 255, 255),  // голубой
    QColor(128, 0, 0),    // темно-красный
    QColor(0, 128, 0),    // темно-зеленый
    QColor(0, 0, 128),    // темно-синий
    QColor(128, 128, 0),  // оливковый
    QColor(128, 0, 128),  // фиолетовый
    QColor(0, 128, 128),  // бирюзовый
    QColor(192, 192, 192), // серебристый
    QColor(255, 165, 0),  // оранжевый
    QColor(255, 105, 180),// розовый
    QColor(139, 69, 19),  // коричневый
    QColor(255, 20, 147), // глубокий розовый
    QColor(34, 139, 34),  // лесной зеленый
    QColor(255, 140, 0),  // золотисто-оранжевый
    QColor(123, 104, 238) // темно-лавандовый
};

double GraphicDrawer::zoomFactor=1.0;
double GraphicDrawer::stepX=80; // scene->width()/10

GraphicDrawer::GraphicDrawer()
{
applyOffset(0,0);
}

TypeOfModel GraphicDrawer::getCurrentModel(const std::string& userInput){
           if( userInput.find("<=") != std::string::npos || userInput.find("< =") != std::string::npos)
               return LESSOREQUAL;
           else if( userInput.find(">=") != std::string::npos || userInput.find("> =") != std::string::npos)
               return MOREOREQUAL;
           else if( userInput.find(">") != std::string::npos || userInput.find("> ") != std::string::npos)
               return MORE;
           else if( userInput.find("<") != std::string::npos || userInput.find("< ") != std::string::npos)
               return LESS;
           else if( userInput.find("=") != std::string::npos)
               return EQUALITY;
           else return UNDEFINEDMODEL;
   }
TypeOfSystem GraphicDrawer::getCurrentSystem(const std::string& userInput){
    if (std::regex_search(userInput, std::regex(R"((^|[^a-zA-Z])r($|[^a-zA-Z]))")))
        return POLAR;
    else
        return CARTESIAN;
    return UNDEFINEDSYSTEM;
}



double findPeriod(const std::string& functionStr) {
    // Ищем тригонометрическую функцию в строке
    std::regex trigRegex("(sin|cos|tan|cot|sec|csc)\\(([^)]+)\\)");
    std::smatch match;

    if (std::regex_search(functionStr, match, trigRegex)) {
        std::string funcType = match[1]; // Тип функции (sin, cos и т.д.)
        std::string argument = match[2]; // Аргумент функции

        // Определяем базовый период
        double basePeriod;
        if (funcType == "sin" || funcType == "cos") {
            basePeriod = 2 * M_PI;
        } else if (funcType == "tan" || funcType == "cot") {
            basePeriod = M_PI;
        } else {
            return 0;
        }

        // Ищем коэффициент перед x и делитель после x
        std::regex coeffRegex(R"(([0-9.]+)\*?theta(?:/([0-9.]+))?)");
        if (std::regex_search(argument, match, coeffRegex)) {
            double coeff = std::stod(match[1]); // Коэффициент перед theta
            double divisor = match[2].matched ? std::stod(match[2]) : 1.0; // Делитель после theta (если есть)

            return basePeriod / (std::abs(coeff) / divisor);
        }

        return basePeriod;
    }

    return 0;
}

void GraphicDrawer::processInequality(QGraphicsScene* scene,QPointF& prevDot,QPointF& currentDot,TypeOfModel currentModel,TypeOfSystem currentSystem,QPointF* boundaryPrev,QPointF* boundaryCurrent,QPointF* center)
{
    QPolygonF fillArea;
    if (currentSystem==CARTESIAN){
        if ((currentModel == LESSOREQUAL ||currentModel == LESS) && currentDot.y()<=scene->height()) {
            fillArea << prevDot
                    << currentDot
                    << QPointF(currentDot.x(),scene->height())
                    << QPointF(prevDot.x(), scene->height());

            QGraphicsPolygonItem* polygonItem = scene->addPolygon(fillArea);
            polygonItem->setBrush(QBrush(QColor(255, 0, 0, 100))); // Полупрозрачный красный цвет
            polygonItem->setPen(Qt::NoPen);

        }
        if ((currentModel == MOREOREQUAL||currentModel == MORE) && currentDot.y()>=0) {
            fillArea << prevDot
                    << currentDot
                    << QPointF(currentDot.x(),0)
                    << QPointF(prevDot.x(), 0);

            QGraphicsPolygonItem* polygonItem = scene->addPolygon(fillArea);
            polygonItem->setBrush(QBrush(QColor(0, 0, 255, 100))); // Полупрозрачный красный цвет
            polygonItem->setPen(Qt::NoPen);
        }

        prevDot = currentDot;
        boundaryPrev = boundaryCurrent;
    }
    if (currentSystem==POLAR){
        if (currentModel == LESSOREQUAL ||currentModel == LESS) {

            fillArea << prevDot
                     << currentDot
                     << *boundaryCurrent
                     << *boundaryPrev;

            // Добавляем полигон на сцену
            QGraphicsPolygonItem* polygonItem = scene->addPolygon(fillArea);
            polygonItem->setBrush(QBrush(QColor(255, 0, 0, 100))); // Полупрозрачный красный цвет
            polygonItem->setPen(Qt::NoPen);


        }
        if (currentModel == MOREOREQUAL||currentModel == MORE) {
            fillArea << prevDot
                     << currentDot
                     << QPointF(center->x(), center->y());

            QGraphicsPolygonItem* polygonItem = scene->addPolygon(fillArea);
            polygonItem->setBrush(QBrush(QColor(0, 0, 255, 100))); // Полупрозрачный синий цвет
            polygonItem->setPen(Qt::NoPen);
        }

        prevDot = currentDot;

    }
}

void GraphicDrawer::handleInequality(QGraphicsScene* scene, QPointF& prevDot, QPointF& currentDot, TypeOfModel currentModel, TypeOfSystem currentSystem, const std::string& userInput, QPointF* center) {
    static QPointF lastCenter; // Сохраняем предыдущее положение центра
    static int counter = 0;
    static double i = 0;
    static QPointF boundaryPrev;

    if (currentSystem == CARTESIAN) {
        processInequality(scene, prevDot, currentDot, currentModel, CARTESIAN);
    } else {
        // Проверяем, изменился ли центр
        if (lastCenter != *center) {
            counter = 0;
            i = 0;
            boundaryPrev = QPointF(
                center->x() + scene->width() * cos(i),
                center->y() - scene->height() * sin(i)
            );
            lastCenter = *center;
        }

        bool isFirstOrSecondSubPeriod = false;
        std::regex trigRegex("([+-]?\\s*(sin|cos|tan|cot|sec|csc))");
        std::smatch match;
        bool hasTrigFunction = std::regex_search(userInput, match, trigRegex);

        if (hasTrigFunction && (currentModel == LESSOREQUAL || currentModel == LESS)) {
            double period = findPeriod(userInput);
            int subPeriodSteps;

            // Устанавливаем количество субпериодов в зависимости от типа функции
            std::string funcType = match[2];
            if (funcType == "sin" || funcType == "cos" || funcType == "sec" || funcType == "csc") {
                subPeriodSteps = 4;
            } else if (funcType == "tan" || funcType == "cot") {
                subPeriodSteps = 2;
            } else {
                throw std::invalid_argument("Unsupported trigonometric function");
            }

            double stepInPeriod = period / subPeriodSteps;
            double currentTheta = fmod(i, period);
            int currentSubPeriod = static_cast<int>(currentTheta / stepInPeriod);

            // Проверка на наличие минусов в выражении
            int minusCount = std::count(userInput.begin(), userInput.end(), '-');
            if (minusCount % 2 != 0) {
                if (funcType == "sin")
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 0 || currentSubPeriod == 2);
                else
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 1 || currentSubPeriod == 2);
            } else {
                if (funcType == "sin")
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 3 || currentSubPeriod == 2);
                else
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 1 || currentSubPeriod == 2);
            }
        }


        if (hasTrigFunction && isFirstOrSecondSubPeriod)
           currentDot=*center;



        QPointF boundaryCurrent(
            center->x() + scene->width() * cos(i),
            center->y() - scene->height() * sin(i)
        );

        if (counter == 0) {
            prevDot = currentDot;
            boundaryPrev = boundaryCurrent;
        }

        processInequality(scene, prevDot, currentDot, currentModel, POLAR, &boundaryPrev, &boundaryCurrent, center);

        if (currentModel == LESSOREQUAL || currentModel == LESS) {
            boundaryPrev = boundaryCurrent;
        }

        counter++;
        if (static_cast<int>(i / M_PI) % 2 == 0 && static_cast<int>(i / M_PI) != 0) {
            counter = 0;
            boundaryPrev = boundaryCurrent;
            i = 0;
        } else {
            i += M_PI / 192;
        }
    }
}

std::vector<QPointF> GraphicDrawer::drawPolarEquation(QGraphicsScene* scene,IDraw* system,Function* function,TypeOfModel currentModel, QPointF center,QPointF* prevDot,double stepX,std::string userInput,int colorIndex){
    QColor currentColor = colors[colorIndex % colors.size()];
    QPointF boundaryPrev;
    QPen Pen;
    std::vector<QPointF> points;
    std::pair<double, double> thetaRange=getThetaRange();
    int counter=0;

    Pen.setColor(currentColor);
    Pen.setWidth(2);

    for(double i =thetaRange.first; i<=thetaRange.second;i+=M_PI/192){
        function->setTheta(i);
        QPointF dot;
        try{
            double r = function->evaluate();
            if (std::isnan(r))
                continue;
            r*=stepX/2;
            points.emplace_back(r*cos(i),-r*sin(i));
            dot=QPointF(center.x()+r*cos(i),center.y()-r*sin(i));
        }catch (const std::domain_error&) {
            continue;
        }
        if(currentModel!=LESS && currentModel!=MORE){
            if(counter)
                scene->addLine(prevDot->x(),prevDot->y(),dot.x(),dot.y(),Pen);
            *prevDot=dot;
            counter++;
        }
    }
    pointsCache[userInput] = points;
    counter=0;

    for (double i = thetaRange.first; i <= thetaRange.second + 0.0001; i += M_PI / 192) {
        bool isFirstOrSecondSubPeriod = false;
        // Проверяем, содержит ли функция тригонометрические функции
        std::regex trigRegex("([+-]?\\s*(sin|cos|tan|cot|sec|csc))");
        std::smatch match;
        bool hasTrigFunction = std::regex_search(userInput, match, trigRegex);

        if (hasTrigFunction && (currentModel == LESSOREQUAL || currentModel == LESS)) {
            double period = findPeriod(userInput);
            int subPeriodSteps;

            // Устанавливаем количество субпериодов в зависимости от типа функции
            std::string funcType = match[2];
            if (funcType == "sin" || funcType == "cos" || funcType == "sec" || funcType == "csc") {
                subPeriodSteps = 4;
            } else if (funcType == "tan" || funcType == "cot") {
                subPeriodSteps = 2;
            } else {
                throw std::invalid_argument("Unsupported trigonometric function");
            }

            double stepInPeriod = period / subPeriodSteps;
            double currentTheta = fmod(i, period);
            int currentSubPeriod = static_cast<int>(currentTheta / stepInPeriod);

            // Проверка на наличие минусов в выражении, для инвертирования субпериодов
            int minusCount = std::count(userInput.begin(), userInput.end(), '-');
            if (minusCount % 2 != 0) {
                if (funcType == "sin")
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 0 || currentSubPeriod == 2);
                else
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 1 || currentSubPeriod == 2);
            } else {
                if (funcType == "sin")
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 3 || currentSubPeriod == 2);
                else
                    isFirstOrSecondSubPeriod = (currentSubPeriod == 1 || currentSubPeriod == 2);
            }
        }

        double r;
        if (hasTrigFunction && isFirstOrSecondSubPeriod)
            r = 0;
        else {
            function->setTheta(i);
            r = function->evaluate();
            if (std::isnan(r)){
                counter=0;
                continue;
            }
            r *= stepX/2;
        }
        QPointF dot(center.x() + r * cos(i), center.y() - r * sin(i));
        QPointF boundaryCurrent(
            center.x() + scene->width() * cos(i),
            center.y() - scene->height() * sin(i)
        );
        if (counter == 0){
            *prevDot = dot;
            boundaryPrev=boundaryCurrent;
        }
        processInequality(scene,*prevDot,dot,currentModel,POLAR,&boundaryPrev,&boundaryCurrent,&center);
        if (currentModel == LESSOREQUAL || currentModel == LESS)
                boundaryPrev = boundaryCurrent;
        counter++;
    }
return points;
}

std::vector<QPointF> GraphicDrawer::drawCartesianEquation(QGraphicsScene* scene, IDraw* system, Function* function, TypeOfModel currentModel, QPointF center, QPointF* prevDot, double stepX, std::string userInput, int colorIndex) {
    std::vector<QPointF> points;
    QColor currentColor = colors[colorIndex % colors.size()];
    QPen Pen(currentColor);
    Pen.setWidth(2);
    int counter = 0;
    for (double i = -system->currentWidth; i <= system->currentWidth; i += 0.001) {
        function->setX(i);
        double valueY = function->evaluate();

        if (std::isnan(valueY)) {
            counter = 0;
            continue;
        }
        points.emplace_back(i * stepX / 2, -valueY * stepX / 2);
        QPointF dot(center.x() + i * stepX / 2, center.y() - valueY * stepX / 2);
        if (currentModel != LESS && currentModel != MORE) {
            if (counter) {
                scene->addLine(prevDot->x(), prevDot->y(), dot.x(), dot.y(), Pen); // Соединяем точки, если нет асимптоты
            }
            *prevDot = dot;
            counter++;
        }

    }
    pointsCache[userInput] = points;
    counter=0;
    for (double i = -system->currentWidth; i <= system->currentWidth; i += 0.001) {
        function->setX(i);
        double valueY = function->evaluate();
        if (std::isnan(valueY))
            continue;
        QPointF dot(center.x() + i * stepX/2, center.y() - valueY * stepX/2);
            if (counter == 0)
                *prevDot = QPointF(center.x() + i * stepX, center.y());
            processInequality(scene,*prevDot,dot,currentModel,CARTESIAN);
            counter++;
    }

return points;
}


std::unordered_map<std::string, std::vector<QPointF>>* GraphicDrawer::drawGraphics(QGraphicsScene *scene, QVector<QLineEdit*>* lineEditVector,
    IDraw* system, std::unordered_map<std::string, std::vector<QPointF>>* pointsCache,int zoomMode,bool wasDrugged,bool wasCalledByButton)
{
    int colorIndex = 0;
    double newZoom = 1;
    int prevZoomMode =0;
    bool first=true;

    QColor currentColor = colors[colorIndex % colors.size()];
    QPointF prevDot;
    TypeOfModel currentModel = EQUALITY;
    TypeOfSystem currentSystem = CARTESIAN;
    QPen Pen;
    Pen.setColor(currentColor);
    Pen.setWidth(2);
    center = scene->sceneRect().center() + offset;

    if(wasDrugged==false){
    // при нулевом режиме зумирования мы просто должны имзенить значения подписей на осях и вернуться к классической сетке
    if (zoomMode == 0) {
        first=false;
        this->stepX = scene->width() / 10;
        newZoom = system->currentWidth/5;
    }
    // режим увеличения масштаба
    else if (zoomMode == 1) {
        this->stepX *= 13.0 / 12;
        this->zoomFactor *= 13.0/12;
    }
    // режим уменьшения масштаба
    else if (zoomMode == 2) {
       this->stepX *= 12.0/ 13;
       this->zoomFactor *=12.0/13;
    }
  }
    static std::unordered_map<std::string, int> firstCacheLoadMap;


    for (QLineEdit* lineEdit : *lineEditVector) {
        if (!lineEdit || lineEdit->text().isEmpty())
            continue;

        QString inputText = lineEdit->text();
        std::string userInput = inputText.toStdString();

        currentColor = colors[colorIndex % colors.size()];
        Pen.setColor(currentColor);

        if (!hasContentChanged(userInput, pointsCache)) {
            currentModel = getCurrentModel(userInput);
            currentSystem = getCurrentSystem(userInput);
            auto it = pointsCache->find(userInput);
            if (it != pointsCache->end()) {
                if (zoomMode == 0 && (!first || wasCalledByButton)) {
                    zoomFactor = 2.0 / newZoom;

                    if (newZoom < 1) {
                        static int checkCount = 2;
                        if (zoomMode == 1) {
                            checkCount = 1.0 / checkCount;
                            zoomFactor = pow(2, checkCount);
                            checkCount = 1.0 / checkCount;
                            checkCount--;
                        } else if (zoomMode == 2) {
                            zoomFactor = pow(2, checkCount);
                            checkCount++;
                        }
                    }
                }

                QPainterPath path;
                QPointF  offsetAdjustedPoint = it->second[0] * zoomFactor + center;
                path.moveTo(offsetAdjustedPoint);

                for (const auto& point : it->second) {
                    bool wasNaN=false;
                    QPointF prevPoint=offsetAdjustedPoint;
                    offsetAdjustedPoint = point * zoomFactor + center;
                    if (std::isnan(offsetAdjustedPoint.y())) {
                        wasNaN=true;
                        continue;
                    }
                    if(currentModel!=LESS && currentModel!=MORE){
                        if(wasNaN)
                            path.moveTo(offsetAdjustedPoint);
                        path.lineTo(offsetAdjustedPoint);
                    }
                    handleInequality(scene,prevPoint,offsetAdjustedPoint,currentModel,currentSystem,userInput,&center);
                }

                scene->addPath(path, Pen);
                colorIndex++;
                prevZoomMode = zoomMode;
                continue;
            }
        }
        Function function(userInput, scene);
        currentModel = getCurrentModel(userInput);

        std::vector<QPointF> computedPoints;

        if (std::regex_search(userInput, std::regex(R"((^|[^a-zA-Z])r($|[^a-zA-Z]))"))) {
            computedPoints = drawPolarEquation(scene, system, &function, currentModel, center, &prevDot, stepX, userInput, colorIndex++);
        } else {
            computedPoints = drawCartesianEquation(scene, system, &function, currentModel, center, &prevDot, stepX, userInput, colorIndex++);
        }

        if (pointsCache == nullptr)
            pointsCache = new std::unordered_map<std::string, std::vector<QPointF>>();

        (*pointsCache)[userInput] = std::move(computedPoints);
    }
    return pointsCache;
}



std::pair<double, double> GraphicDrawer::getThetaRange(QWidget* parent) {
    QDialog dialog(parent);
    dialog.setWindowTitle("Выберите диапазон θ");

    QVBoxLayout layout(&dialog);

    QLabel* labelMin = new QLabel("Минимальное значение θ (Pi):", &dialog);
    QDoubleSpinBox* minTheta = new QDoubleSpinBox(&dialog);
    minTheta->setRange(0, 12);
    minTheta->setSingleStep(0.5);

    QLabel* labelMax = new QLabel("Максимальное значение θ (Pi):", &dialog);
    QDoubleSpinBox* maxTheta = new QDoubleSpinBox(&dialog);
    maxTheta->setRange(0, 12 );
    maxTheta->setSingleStep(0.5);
    maxTheta->setValue(2);

    QPushButton* okButton = new QPushButton("OK", &dialog);

    layout.addWidget(labelMin);
    layout.addWidget(minTheta);
    layout.addWidget(labelMax);
    layout.addWidget(maxTheta);
    layout.addWidget(okButton);

    QObject::connect(okButton, &QPushButton::clicked, [&dialog]() {
        dialog.accept();
    });

    dialog.exec();

    return {minTheta->value()*M_PI, maxTheta->value()*M_PI};
}

std::string normalizeInput(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
    return result;
}

bool GraphicDrawer::hasContentChanged(const std::string& key, const std::unordered_map<std::string, std::vector<QPointF>>* pointsCache) {
    auto normalizedKey = normalizeInput(key);
    if(pointsCache!=nullptr)
    return pointsCache->find(normalizedKey) == pointsCache->end();
    else return true;
}
