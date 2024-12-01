#ifdef POLARHEADER_H
#define POLARHEADER_H

#include<vector>
#include<QGraphicsScene>
#include<QGraphicsTextItem>
#include<cmath>
#include<QtMath>


class DrawPolar{
public:
    DrawPolar(){currentWidth=10,currentHeight=10;};
    double currentHeight;
    double currentWidth;
    std::vector<double> getCurrentBorders();
    void setCurrentBorders(double newWidth,double newHeight);
    void setPolar(QGraphicsScene * scene, double height,double width,int zoomMode,double dx,double dy);
    void drawAxis(QGraphicsScene * scene,double dx,double dy);
    void drawGrid(QGraphicsScene *scene,double stepX,double stepY,bool isEven,double dx,double dy);
    void divisionsDrawing(QGraphicsScene *scene,double stepX,double stepY,double dx,double dy);
    void nameDivisions(QGraphicsScene *scene,double stepX,double stepY,double dx,double dy);
    void addAngleLabels(QGraphicsScene *scene, const QPointF &center, double radius, double dx, double dy);

};


#endif // POLARHEADER_H
