#ifndef IDRAW_H
#define IDRAW_H
#include<vector>
#include<QGraphicsScene>
#include<QGraphicsTextItem>
#include<cmath>
#include<QtMath>
#include<QPointF>

class IDraw
{
public:
    virtual ~IDraw() = default;
    double currentHeight;
    double currentWidth;
    static double dx;
    static double dy;
    bool wasDrugged=false;
    void applyOffset(double dx, double dy) {
        offset += QPointF(dx, dy);
    }
    QPointF getOffset(){
        return offset;
    }
QPointF getCenter(){return center;}
    std::vector<double> getCurrentBorders();
    void setCurrentBorders(double newWidth, double newHeight);
    void setSystem(QGraphicsScene* scene, double height, double width, int zoomMode,bool wasDrugged, double dx, double dy);
    void drawAxis(QGraphicsScene* scene);
    void nameDivisions(QGraphicsScene* scene,double stepX);
    virtual void drawGrid(QGraphicsScene* scene,double stepX, double stepY, bool isEven,double dx,double dy) = 0;
    virtual void divisionsDrawing(QGraphicsScene* scene,double stepX, double stepY) = 0;
protected:
    QPointF offset = QPointF(0, 0);
    QPointF center;

};




class DrawCartesian : public IDraw {
public:
    DrawCartesian() { currentWidth = 10, currentHeight = 10; };
    void drawGrid(QGraphicsScene* scene,double stepX, double stepY, bool isEven,double dx,double dy) override;
    void divisionsDrawing(QGraphicsScene* scene,double stepX, double stepY) override;
};




class DrawPolar : public IDraw {
public:

    DrawPolar() { currentWidth = 10, currentHeight = 10; };
    void drawGrid(QGraphicsScene* scene,double stepX, double stepY, bool isEven,double dx,double dy) override;
    void divisionsDrawing(QGraphicsScene* scene,double stepX, double stepY) override;
    void addAngleLabels(QGraphicsScene* scene, const QPointF& center, double radius, double dx, double dy);
};

#endif // IDRAW_H
