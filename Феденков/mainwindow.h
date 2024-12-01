#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//#include "DrawCartesian.h"
//#include "DrawPolar.h"
#include"IDraw.h"
#include "mygraphicsview.h"
#include "GraphicDrawer.h"
#include"FileManager.h"
#include"myqlineedit.h"
#include <QMainWindow>
#include<QGraphicsScene>
#include <QGraphicsView>
#include<QPushButton>
#include<QHBoxLayout>
#include <QVBoxLayout>
#include<QMouseEvent>
#include<QWidget>
#include<QLineEdit>
#include <QSignalMapper>
#include <QDialog>
#include<QLabel>
#include <QGroupBox>

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

void resizeEvent(QResizeEvent *event) override;

void keyPressEvent(QKeyEvent *event)override;
void addLineEdit();
void drawGraphics();
void onToggleSystemButtonClicked();
void addLineEditButtonClicked();
void initializeFunctionButton();
void openFunctionDialog();
void addFunctionToLineEdit(const QString &text);
void addButtonsToLayout(const QStringList &buttons, QGridLayout *layout, QDialog *dialog);
void clearAll();
void zoomInEvent();
void zoomOutEvent();
void loadGraphics();
void saveGraphics();
private slots:
void onLineEditFocused();
void addNewLineEdit();
void moveCursorLeft();
void moveCursorRight();
void deleteLastCharacter();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    IDraw* system;
    GraphicDrawer* drawer;
    QVector<QLineEdit*> lineEditVector;
    CustomGraphicsView  *view;
    QHBoxLayout *mainLayout;
    QVBoxLayout *userInputLayout;
    QLineEdit *lastActiveLineEdit = nullptr;
    QWidget *inputPanel;
    FileManager *fileManager;
    QLineEdit *thetaRangeInput;
    QLineEdit *currentWidthInput;
    int inputCount;
    std::unordered_map<std::string, std::vector<QPointF>>* pointsCache;

};

#endif // MAINWINDOW_H
