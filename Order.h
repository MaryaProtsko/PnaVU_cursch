#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QVector>

#include "Product.h"

class Order
{
private:
    int orderId;
    QString customerName;
    QVector<Product> products;
    double totalPrice;

public:
    Order(int orderId, const QString& customerName, const QVector<Product>& products, double totalPrice)
        : orderId(orderId), customerName(customerName), products(products), totalPrice(totalPrice) {}

    void displayOrderInfo() const;
};

#endif // ORDER_H
