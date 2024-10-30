#include "gamefield.h"
#include "ui_gamefield.h"
#include "generation.h"
GameField::GameField(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GameField)
{
    sudoku s;
    s.generator();
    QTextEdit* a;
    QFont font;
    this->setFixedSize(400,400);
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            buttons[i][j]=new QTextEdit(this);
            buttons[i][j]->setText(" " + QString::number(s.basic_field[i][j]));
            buttons[i][j]->setFixedSize(40,40);
            font = buttons[i][j]->font();
            font.setPointSize(16); // Устанавливаем размер шрифта
            buttons[i][j]->setFont(font);
            buttons[i][j]->setStyleSheet("QTextEdit{background-color:Honeydew;border: 1px solid gray;}");
            buttons[i][j]->setCursor(Qt::PointingHandCursor);
            buttons[i][j]->move(40*j,40*i);
            buttons[i][j]->setCursorWidth(-18708910);
             connect( buttons[i][j], &QTextEdit::textChanged, this, &GameField::keepLastValue);
        }
    }
    ui->setupUi(this);
}

GameField::~GameField()
{
    delete ui;
}

void GameField::keepLastValue(){
      QTextEdit *button = qobject_cast<QTextEdit*>(sender());
   QString currentText = button->toPlainText();
 QChar lastChar = currentText[currentText.length() - 1];
    // Очищаем текстовое поле
    button->blockSignals(true);
    button->clear();
    // Устанавливаем только последнее введённое значение
    if(lastChar >='1'&&lastChar<='9'){
        button->setText(' '+lastChar);
    }
    button->blockSignals(false);
};
/*void GameField::keyPressEvent(QKeyEvent *e)
{
    if(button_pressed==true){
        if (e->key() >= Qt::Key_0 && e->key() <= Qt::Key_9) {
            str=e->text();
            number=str.toInt();
            button_pressed=false;
        }
    }
}*/

/*void GameField::ClickedSlot()
{
    number=0;
    button_pressed=true;
  QPushButton *button = qobject_cast<QPushButton *>(sender());
    button->setStyleSheet("QPushButton{background-color:White}");
    button->setText(str);
    //button->setText("писюн");
    qDebug("ABOBA");
}*/


