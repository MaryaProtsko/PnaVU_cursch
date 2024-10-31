#ifndef STORE_H
#define STORE_H

#include <QVector>

#include "Category.h"
#include "Order.h"
#include "User.h"
#include "Customer.h"
#include "Cart.h"

class Store
{
private:
    QVector<Category> categories;
    QVector<Order> orders;
    QVector<User*> users;

public:
    void addCategory(const Category& category);
    void registerUser(User* user);
    void createOrder(const Customer& customer, const Cart& cart);
};

#endif // STORE_H
