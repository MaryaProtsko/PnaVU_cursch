#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DrawCartesian cartesianScene;
    cartesianScene.setCurrentBorders(10,10);
    cartesianScene.setCartesian(w.getScene(),cartesianScene.getCurrentBorders()[0],cartesianScene.getCurrentBorders()[1],0,0,0);
    w.show();
    return a.exec();
}
