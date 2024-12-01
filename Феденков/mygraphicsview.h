#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <QGraphicsView>
#include<QGraphicsScene>
#include <QMouseEvent>
#include"IDraw.h"
#include"GraphicDrawer.h"

class CustomGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CustomGraphicsView(QGraphicsScene *scene, IDraw* system,GraphicDrawer* drawer, QVector<QLineEdit*>* lineEditVector, QWidget *parent = nullptr);
    void setLineEditVector( QVector<QLineEdit*>* lineEditVectorS);
    void setPointsCache(std::unordered_map<std::string, std::vector<QPointF>>* pointsCache);
    int getZoomMode();
    bool getWasDrugged();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event)override;

private:
    IDraw* system;
    GraphicDrawer* drawer;
    QVector<QLineEdit*>* lineEditVector;
    std::unordered_map<std::string, std::vector<QPointF>>* pointsCache;
    QPointF center;
    QPoint lastMousePosition;
    int zoomMode;
    bool isDragging = false;
    bool wasDrugged;
};

#endif // MYGRAPHICSVIEW_H
