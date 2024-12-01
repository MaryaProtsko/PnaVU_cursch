#include "mygraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, DrawCartesian *sharedCartesian, QWidget *parent)
    : QGraphicsView(scene, parent), cartesian(sharedCartesian) {}


void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        if (!isDragging) {
            isDragging = true;
            lastMousePosition = event->pos();
        }
        QPoint currentMousePosition = event->pos();
        double dx = currentMousePosition.x() - lastMousePosition.x();
        double dy = currentMousePosition.y() - lastMousePosition.y();

        scene()->clear();
        cartesian->setCartesian(scene(), cartesian->currentHeight, cartesian->currentWidth, 0, dx, dy);

        lastMousePosition = currentMousePosition;
    } else {
        isDragging = false;
    }
}


void CustomGraphicsView::wheelEvent(QWheelEvent *event){
     std::vector<double> currentBorders=cartesian->getCurrentBorders();
     double zoomInScaleFactor=2;
     double zoomOutScaleFactor=2;
     static int degrees=0;
     if((degrees>0 &&(event->angleDelta().y() / 8)>0) || (degrees<0 && (event->angleDelta().y() / 8)<0))
       degrees += event->angleDelta().y() / 8;
     else{
         degrees=0;
         degrees+=event->angleDelta().y() / 8;
     }
    if(degrees > 0 && degrees==90){
        degrees=0;
         scene()->clear();
        cartesian->setCartesian( scene(),currentBorders[0]/zoomInScaleFactor,currentBorders[1]/zoomInScaleFactor,0,0,0);
    }
    else if(degrees > 0 && degrees!=90){
         scene()->clear();
        cartesian->setCartesian( scene(),currentBorders[0],currentBorders[1],1,0,0);
    }
    else if(degrees < 0 &&degrees==-90){
        degrees=0;
         scene()->clear();
        cartesian->setCartesian( scene(),currentBorders[0]*zoomOutScaleFactor,currentBorders[1]*zoomOutScaleFactor,0,0,0);
    }
    else if(degrees < 0 &&degrees!=-90){
         scene()->clear();
        cartesian->setCartesian( scene(),currentBorders[0],currentBorders[1],2,0,0);
    }
}


void CustomGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {

    scene()->clear();
    cartesian->setCartesian(scene(),10,10,0,0,0);
}
