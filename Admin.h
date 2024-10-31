#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Category.h"
#include "Product.h"

class Admin : public User
{
public:
    Admin(const QString& name, const QString& surname, const QString& email)
        : User(name, surname, email) {}

    void displayRole() const override;

    void addProduct(Category& category, const Product& product);
};

#endif // ADMIN_H
