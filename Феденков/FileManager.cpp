#include "FileManager.h"
#include <QFileDialog>
#include <QPainter>

FileManager::FileManager(QGraphicsScene *scene, QVector<QLineEdit*>*lineEditVector, QVBoxLayout *layout)
    : scene(scene), lineEditVector(lineEditVector), userInputLayout(layout) {}

void FileManager::saveGraphics() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Сохранить графики"), "./", QObject::tr("Images (*.jpg)"));
    if (fileName.isEmpty()) return;

    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    scene->render(&painter);
    image.save(fileName);

    QFile textFile(fileName + ".txt");
    if (textFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&textFile);
        for (const auto &lineEdit : *lineEditVector) {
            if(lineEdit->text()!="")
            out << lineEdit->text() << "\n";
        }
        textFile.close();
    }
}

void FileManager::loadGraphics() {
    QString fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Загрузить графики"), "./", QObject::tr("Text Files (*.txt)"));
    if (fileName.isEmpty()) return;

    QFile textFile(fileName);
    if (textFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&textFile);

        while (!in.atEnd()) {
            QString line = in.readLine();
            MyLineEdit *lineEdit = new MyLineEdit(nullptr);
            lineEdit->setText(line);
            userInputLayout->addWidget(lineEdit);
            lineEditVector->push_back(lineEdit);
        }
        textFile.close();
    }
}

