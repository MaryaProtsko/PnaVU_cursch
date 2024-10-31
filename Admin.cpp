#include <QDebug>

#include "Admin.h"

void Admin::displayRole() const
{
    qDebug() << "Role: Admin";
}

void Admin::addProduct(Category& category, const Product& product)
{
    category.addProduct(product);
    qDebug() << "Product added by admin: " << name;
}
