#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QDialog>
#include <QPushButton>
#include <QBoxLayout>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPainter>
#include<QTextEdit>
#include<QCursor>
namespace Ui {
class GameField;
}

class GameField : public QDialog
{
    Q_OBJECT

public:
    explicit GameField(QWidget *parent = nullptr);
    ~GameField();

private:
    Ui::GameField *ui;
    QTextEdit *buttons[9][9];
public:
   // void keyPressEvent(QKeyEvent*e)override;
public slots:
    void keepLastValue();
    //void ClickedSlot();
};

#endif // GAMEFIELD_H
