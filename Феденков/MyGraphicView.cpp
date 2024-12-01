#include "mygraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, IDraw* systemS,GraphicDrawer* drawerS, QVector<QLineEdit*>* lineEditVectorS, QWidget *parent)
    : QGraphicsView(scene, parent),system(systemS),drawer(drawerS),lineEditVector(lineEditVectorS),zoomMode(0)
{
    center=QPointF(400,300);
}

void CustomGraphicsView::setLineEditVector( QVector<QLineEdit*>* lineEditVectorS){
     lineEditVector=lineEditVectorS;
}

void CustomGraphicsView::setPointsCache(std::unordered_map<std::string, std::vector<QPointF>>* pointsCacheS){
    pointsCache=pointsCacheS;
}

int CustomGraphicsView::getZoomMode(){
    return zoomMode;
}
bool CustomGraphicsView::getWasDrugged(){
return wasDrugged;
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    std::unordered_map<std::string, std::vector<QPointF>>* p=pointsCache;
  static  bool zoomModeApplied=false;
    if (event->buttons() & Qt::LeftButton) {
        if (!isDragging) {
            isDragging = true;
            lastMousePosition = event->pos();
        }

        QPoint currentMousePosition = event->pos();
        double dx = currentMousePosition.x() - lastMousePosition.x();
        double dy = currentMousePosition.y() - lastMousePosition.y();
        lastMousePosition = currentMousePosition;

        // Передаем смещение в систему
        system->applyOffset(dx, dy);
        drawer->applyOffset(dx,dy);

        scene()->clear();

        // Перерисовка
        if(!zoomModeApplied){
            zoomModeApplied=true;
            wasDrugged =false;
            system->setSystem(scene(), system->currentHeight, system->currentWidth, zoomMode,false,dx,dy);
            pointsCache = drawer->drawGraphics(scene(), lineEditVector, system,pointsCache,zoomMode,true);
        }
        else{
            wasDrugged = true;
            system->setSystem(scene(), system->currentHeight, system->currentWidth, 0,true,dx,dy);
            pointsCache = drawer->drawGraphics(scene(), lineEditVector, system,pointsCache,0,true);
        }
    } else {
        isDragging = false;
    }
}



void CustomGraphicsView::wheelEvent(QWheelEvent *event){
     std::vector<double> currentBorders = system->getCurrentBorders();
     double zoomScaleFactor=2;
     static int degrees=0;
     if((degrees>0 &&(event->angleDelta().y() / 8)>0) || (degrees<0 && (event->angleDelta().y() / 8)<0))
       degrees += event->angleDelta().y() / 8;
     else{
         degrees=0;
         degrees+=event->angleDelta().y() / 8;
     }
    if(degrees > 0 && degrees==90){
         zoomMode = 0;
        degrees=0;
         scene()->clear();
          system->setSystem(scene(),currentBorders[0] / zoomScaleFactor, currentBorders[1] / zoomScaleFactor, zoomMode,false, system->dx, system->dy);
    }
    else if(degrees > 0 && degrees!=90){
          zoomMode = 1;
          scene()->clear();
          system->setSystem(scene(), currentBorders[0], currentBorders[1], zoomMode,false, system->dx, system->dy);
       }
    else if(degrees < 0 &&degrees==-90){
         zoomMode = 0;
        degrees=0;
         scene()->clear();
         system->setSystem( scene(),currentBorders[0] * zoomScaleFactor, currentBorders[1] * zoomScaleFactor, zoomMode,false, system->dx, system->dy);
    }
    else if(degrees < 0 &&degrees!=-90){
         zoomMode = 2;
         scene()->clear();
          system->setSystem(scene(), currentBorders[0], currentBorders[1], zoomMode,false, system->dx, system->dy);
    }
    pointsCache = drawer->drawGraphics(scene(), lineEditVector, system,pointsCache,zoomMode,false);
}


void CustomGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {

    scene()->clear();
     system->setSystem(scene(),10, 10, 0,false, 0, 0);
}
