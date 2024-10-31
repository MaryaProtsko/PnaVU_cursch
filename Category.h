#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <QVector>

#include "Product.h"

class Category
{
private:
    QString name;
    QVector<Product> products;

public:
    Category(const QString& name) : name(name) {}

    void setName(const QString& name);
    QString getName() const;

    void addProduct(const Product& product);
    void displayProducts() const;
};

#endif // CATEGORY_H
