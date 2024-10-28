//
// Created by Daniil on 28.10.2024.
//

#include "Button.h"

Button::Button(const QString &text, QWidget *parent) : QPushButton(text, parent) {
    setCursor(Qt::PointingHandCursor);
    setStyleSheet("QPushButton {"
                  "border-radius: 15px;"  // Указываем радиус закругления
                  "background-color: #4CAF50;"  // Цвет фона
                  "color: white;"  // Цвет текста
                  "padding: 10px;"
                  "font-size: 16px;"
                  "}"
                  "QPushButton:hover {"
                  "background-color: #45a049;"  // Цвет фона при наведении
                  "}");
}
