#include <QDebug>

#include "Customer.h"

void Customer::displayRole() const
{
    qDebug() << "Role: Customer";
}

void Customer::addProductToCart(const Product& product)
{
    cart.addItem(product);
    qDebug() << name << "added product to cart: " << product.getName();
}

void Customer::checkout()
{
    qDebug() << "Customer: " << name << " has checked out.";
    cart.displayCart();
}
