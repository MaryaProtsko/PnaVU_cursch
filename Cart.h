#ifndef CART_H
#define CART_H

#include <QVector>

#include "Product.h"

class Cart
{
private:
    QVector<Product> items;

public:
    QVector<Product> getItems() const;

    void addItem(const Product& product);
    void removeItem(int productId);
    double calculateTotal() const;
    void displayCart() const;
};

#endif // CART_H
