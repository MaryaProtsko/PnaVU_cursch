#include <QDebug>

#include "Category.h"

void Category::setName(const QString& name)
{
    this->name = name;
}

QString Category::getName() const
{
    return this->name;
}

void Category::addProduct(const Product& product)
{
    products.push_back(product);
}

void Category::displayProducts() const
{
    qDebug() << "Category: " << name;
    for (const auto& product : products) {
        product.displayProductInfo();
        qDebug() << "---------------";
    }
}
