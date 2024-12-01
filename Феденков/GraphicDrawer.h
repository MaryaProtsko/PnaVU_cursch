#ifndef GRAPHICDRAWER_H
#define GRAPHICDRAWER_H
#include<QGraphicsScene>
#include<Function.h>
#include<QLineEdit>
#include"IDraw.h"

enum TypeOfModel{
    EQUALITY,LESS,MORE,LESSOREQUAL,MOREOREQUAL,UNDEFINEDMODEL
};

enum TypeOfSystem{
    CARTESIAN,POLAR,UNDEFINEDSYSTEM
};

struct DrawerParams {
    QGraphicsScene* scene;
    QPointF& prevDot;
    QPointF& currentDot;
    TypeOfModel currentModel;
    TypeOfSystem currentSystem;
    std::string userInput;
    QPointF* center;
};

class GraphicDrawer
{
public:
    QPointF offset = QPointF(0, 0);
    QPointF center;
    static double zoomFactor;
    static double stepX;
    void applyOffset(double dx, double dy) {
        offset += QPointF(dx, dy);
    }
    GraphicDrawer();
    std::unordered_map<std::string, std::vector<QPointF>>* drawGraphics(QGraphicsScene *scene, QVector<QLineEdit*>* lineEditVector,
                                                                        IDraw* system, std::unordered_map<std::string,
                                                                        std::vector<QPointF>>* pointsCache,int zoomMode,bool wasDrugged,bool wasCalledByButton=false);
    void handleInequality(QGraphicsScene* scene, QPointF& prevDot, QPointF& currentDot, TypeOfModel currentModel, TypeOfSystem currentSystem, const std::string& userInput, QPointF* center);
    void processInequality(QGraphicsScene* scene,QPointF& prevDot,QPointF& currentDot,TypeOfModel currentModel,TypeOfSystem currentSystem,QPointF* boundaryPrev=nullptr,QPointF* boundaryCurrent=nullptr,QPointF* center=nullptr);
    TypeOfModel getCurrentModel(const std::string& userInput);
    TypeOfSystem getCurrentSystem(const std::string& userInput);
    std::vector<QPointF> drawPolarEquation(QGraphicsScene* scene,IDraw* system,Function* function,TypeOfModel currentModel, QPointF center,QPointF* prevDot,double stepX,std::string userInput,int colorIndex);
    std::vector<QPointF> drawCartesianEquation(QGraphicsScene* scene,IDraw* system,Function* function,TypeOfModel currentModel, QPointF center,QPointF* prevDot,double stepX,std::string userInput,int colorIndex);
    bool hasContentChanged(const std::string& key, const std::unordered_map<std::string, std::vector<QPointF>>* pointsCache);
    std::pair<double, double> getThetaRange(QWidget* parent=nullptr);
    std::unordered_map<std::string, std::vector<QPointF>> pointsCache;

};

#endif // GRAPHICDRAWER_H
