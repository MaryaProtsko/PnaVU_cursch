#include <algorithm>
#include <QDebug>

#include "Cart.h"

QVector<Product> Cart::getItems() const
{
    return this->items;
}

void Cart::addItem(const Product& product)
{
    items.push_back(product);
}

void Cart::removeItem(int productId)
{
    items.erase(std::remove_if(items.begin(), items.end(), [productId](Product& product) {
        return product.getId() == productId;
    }), items.end());
}

double Cart::calculateTotal() const
{
    double total = 0;
    for (const auto& item : items) {
        total += item.getPrice();
    }
    return total;
}

void Cart::displayCart() const
{
    qDebug() << "Cart items: ";
    for (const auto& item : items) {
        item.displayProductInfo();
    }
    qDebug() << "Total price: $" << calculateTotal();
}
