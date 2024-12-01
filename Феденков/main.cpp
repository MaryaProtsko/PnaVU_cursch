#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Constant");
   // w.setWindowIcon();
    DrawCartesian* startScene = new DrawCartesian();
    startScene->setSystem(w.getScene(),startScene->getCurrentBorders()[0],startScene->getCurrentBorders()[1],0,false,0,0);

    w.show();
    return a.exec();
}
