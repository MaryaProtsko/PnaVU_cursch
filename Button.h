//
// Created by Daniil on 28.10.2024.
//

#ifndef QTPROJECT_BUTTON_H
#define QTPROJECT_BUTTON_H

#include <QPushButton>

class Button : public QPushButton {
    Q_OBJECT
public:
    explicit Button(const QString &text, QWidget *parent = nullptr);
};


#endif //QTPROJECT_BUTTON_H
