#include "Store.h"

void Store::addCategory(const Category& category)
{
    categories.push_back(category);
}

void Store::registerUser(User* user)
{
    users.push_back(user);
}

void Store::createOrder(const Customer& customer, const Cart& cart)
{
    int orderId = orders.size() + 1;
    Order newOrder(orderId, customer.getName(), cart.getItems(), cart.calculateTotal());
    orders.push_back(newOrder);
    newOrder.displayOrderInfo();
}
