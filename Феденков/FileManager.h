#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QFile>
#include <QVector>
#include <QVBoxLayout>
#include <QTextStream>
#include "myqlineedit.h"

class FileManager {
public:
    explicit FileManager(QGraphicsScene *scene, QVector<QLineEdit*>* lineEditVector, QVBoxLayout *layout);

    void saveGraphics();
    void loadGraphics();

private:
    QGraphicsScene *scene;
    QVector<QLineEdit*>* lineEditVector;
    QVBoxLayout *userInputLayout;
};

#endif // FILEMANAGER_H


