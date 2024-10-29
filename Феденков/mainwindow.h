#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include"Cartesian.h"
#include "mygraphicsview.h"
#include <QMainWindow>
#include<QGraphicsScene>
#include <QGraphicsView>
#include<QHBoxLayout>
#include <QVBoxLayout>
#include<QMouseEvent>
#include<QWidget>
#include<QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsScene* getScene() const;
    QGraphicsView* getView() const;
    void setScene(QGraphicsScene* sceneToSet);
    void setView(QGraphicsView* viewToSet);


void resizeEvent(QResizeEvent *event) override;

void keyPressEvent(QKeyEvent *event)override;
void addLineEdit();
void drawGraphics();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    DrawCartesian* cartesian;
    QGraphicsView  *view;
    QHBoxLayout *mainLayout;
    QVBoxLayout *userInputLayout;
    QWidget *inputPanel;
    int inputCount;

};



#endif // MAINWINDOW_H
