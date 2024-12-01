#ifdef CARTESIAN_H
#define CARTESIAN_H
#include<vector>
#include <QGraphicsTextItem>
#include<QGraphicsScene>

class DrawCartesian{
public:
    DrawCartesian(){currentWidth=10,currentHeight=10;};
    double currentHeight;
    double currentWidth;
    std::vector<double> getCurrentBorders();
    void setCurrentBorders(double newWidth,double newHeight);
    void setCartesian(QGraphicsScene * scene, double height,double width,int zoomMode,double dx,double dy);
    void drawAxis(QGraphicsScene * scene,double dx,double dy);
    void drawGrid(QGraphicsScene *scene,double stepX,double stepY,bool isEven,double dx,double dy);
    void divisionsDrawing(QGraphicsScene *scene,double stepX,double stepY,double dx,double dy);
    void nameDivisions(QGraphicsScene *scene,double stepX,double stepY,double dx,double dy);
};




#endif // CARTESIAN_H
