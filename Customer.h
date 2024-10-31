#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include "Cart.h"
#include "Product.h"

class Customer : public User
{
private:
    Cart cart;

public:
    Customer(const QString& name, const QString& surname, const QString& email)
        : User(name, surname, email) {}

    void displayRole() const override;

    void addProductToCart(const Product& product);
    void checkout();
};

#endif // CUSTOMER_H
