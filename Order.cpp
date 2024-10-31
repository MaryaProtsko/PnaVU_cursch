#include <QDebug>

#include "Order.h"

void Order::displayOrderInfo() const
{
    qDebug() << "Order ID: " << orderId << "\nCustomer: " << customerName <<
        "\nTotal price: $" << totalPrice;
    for (const auto& product : products) {
        product.displayProductInfo();
    }
}
