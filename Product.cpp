#include <QDebug>

#include "Product.h"

int Product::getId() const
{
    return this->id;
}

QString Product::getName() const
{
    return this->name;
}

QString Product::getDescription() const
{
    return this->description;
}

double Product::getPrice() const
{
    return this->price;
}

int Product::getStock() const
{
    return this->stock;
}

void Product::setId(int id)
{
    this->id = id;
}

void Product::setName(const QString& name)
{
    this->name = name;
}

void Product::setDescription(const QString& description)
{
    this->description = description;
}

void Product::setPrice(double price)
{
    this->price = price;
}

void Product::setStock(int stock)
{
    this->stock = stock;
}

void Product::displayProductInfo() const
{
    qDebug() << "Product ID: " << id << "\nName: " << name <<
        "\nDescription: " << description << "\nPrice: $" << price
             << "\nStock: " << stock;
}
