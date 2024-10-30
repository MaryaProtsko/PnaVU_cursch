/********************************************************************************
** Form generated from reading UI file 'gamefield.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEFIELD_H
#define UI_GAMEFIELD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_GameField
{
public:
    QPushButton *pushButton_57;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QFrame *line_5;
    QFrame *line_6;
    QFrame *line_7;
    QFrame *line_8;
    QLineEdit *lineEdit;
    QTextEdit *textEdit;

    void setupUi(QDialog *GameField)
    {
        if (GameField->objectName().isEmpty())
            GameField->setObjectName("GameField");
        GameField->resize(935, 592);
        GameField->setAutoFillBackground(false);
        pushButton_57 = new QPushButton(GameField);
        pushButton_57->setObjectName("pushButton_57");
        pushButton_57->setGeometry(QRect(160, 360, 50, 50));
        pushButton_57->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        pushButton_57->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color:Honeydew;\n"
"border: 1px solid gray;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color:White;\n"
"}"));
        line = new QFrame(GameField);
        line->setObjectName("line");
        line->setGeometry(QRect(111, 0, 16, 360));
        line->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        line->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line->setFrameShadow(QFrame::Shadow::Plain);
        line->setLineWidth(6);
        line->setFrameShape(QFrame::Shape::VLine);
        line_2 = new QFrame(GameField);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(231, 0, 16, 360));
        line_2->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        line_2->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line_2->setFrameShadow(QFrame::Shadow::Plain);
        line_2->setLineWidth(6);
        line_2->setFrameShape(QFrame::Shape::VLine);
        line_3 = new QFrame(GameField);
        line_3->setObjectName("line_3");
        line_3->setGeometry(QRect(354, 0, 16, 360));
        line_3->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        line_3->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line_3->setFrameShadow(QFrame::Shadow::Plain);
        line_3->setLineWidth(6);
        line_3->setFrameShape(QFrame::Shape::VLine);
        line_4 = new QFrame(GameField);
        line_4->setObjectName("line_4");
        line_4->setGeometry(QRect(0, 117, 360, 6));
        line_4->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line_4->setFrameShadow(QFrame::Shadow::Plain);
        line_4->setLineWidth(6);
        line_4->setFrameShape(QFrame::Shape::HLine);
        line_5 = new QFrame(GameField);
        line_5->setObjectName("line_5");
        line_5->setGeometry(QRect(0, 237, 360, 6));
        line_5->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line_5->setFrameShadow(QFrame::Shadow::Plain);
        line_5->setLineWidth(6);
        line_5->setFrameShape(QFrame::Shape::HLine);
        line_6 = new QFrame(GameField);
        line_6->setObjectName("line_6");
        line_6->setGeometry(QRect(0, 357, 365, 6));
        line_6->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line_6->setFrameShadow(QFrame::Shadow::Plain);
        line_6->setLineWidth(6);
        line_6->setFrameShape(QFrame::Shape::HLine);
        line_7 = new QFrame(GameField);
        line_7->setObjectName("line_7");
        line_7->setGeometry(QRect(0, 0, 360, 3));
        line_7->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line_7->setFrameShadow(QFrame::Shadow::Plain);
        line_7->setLineWidth(3);
        line_7->setFrameShape(QFrame::Shape::HLine);
        line_8 = new QFrame(GameField);
        line_8->setObjectName("line_8");
        line_8->setGeometry(QRect(0, 0, 3, 360));
        line_8->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        line_8->setStyleSheet(QString::fromUtf8("color:MediumAquamarine;"));
        line_8->setFrameShadow(QFrame::Shadow::Plain);
        line_8->setLineWidth(6);
        line_8->setFrameShape(QFrame::Shape::VLine);
        lineEdit = new QLineEdit(GameField);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(240, 360, 113, 28));
        lineEdit->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit{background-color:Honeydew;border: 1px solid gray;}\n"
"input {\n"
"  padding: 10px;\n"
"  caret-color: auto;\n"
"}"));
        lineEdit->setCursorPosition(0);
        lineEdit->setCursorMoveStyle(Qt::CursorMoveStyle::LogicalMoveStyle);
        textEdit = new QTextEdit(GameField);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(380, 280, 104, 85));
        textEdit->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        textEdit->setLineWidth(0);
        textEdit->setLineWrapColumnOrWidth(-6);
        textEdit->setCursorWidth(-18708910);

        retranslateUi(GameField);

        QMetaObject::connectSlotsByName(GameField);
    } // setupUi

    void retranslateUi(QDialog *GameField)
    {
        GameField->setWindowTitle(QCoreApplication::translate("GameField", "Dialog", nullptr));
        pushButton_57->setText(QString());
        lineEdit->setText(QString());
        textEdit->setHtml(QCoreApplication::translate("GameField", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameField: public Ui_GameField {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEFIELD_H
