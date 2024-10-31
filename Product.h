#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product
{
private:
    int id;
    QString name;
    QString description;
    double price;
    int stock;

public:
    Product(int id, const QString& name, const QString& description, double price, int stock)
        : id(id), name(name), description(description), price(price), stock(stock) {}

    int getId() const;
    QString getName() const;
    QString getDescription() const;
    double getPrice() const;
    int getStock() const;

    void setId(int id);
    void setName(const QString& name);
    void setDescription(const QString& description);
    void setPrice(double price);
    void setStock(int stock);

    void displayProductInfo() const;
};

#endif // PRODUCT_H
