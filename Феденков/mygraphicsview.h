#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QMouseEvent>
#include "Cartesian.h"

class CustomGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CustomGraphicsView(QGraphicsScene *scene,DrawCartesian* sharedCartesian, QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event)override;

private:
    DrawCartesian *cartesian; // Указатель на DrawCartesian, чтобы иметь доступ к сетке
    QPoint lastMousePosition;
    bool isDragging = false;
};

#endif // MYGRAPHICSVIEW_H
