/*
Класс военская часть в которой название части и кто в ней состоит
*/
#ifndef MILLITARYUNIT_H
#define MILLITARYUNIT_H
#include "conscript.h"
#include <QString>
#include <QVector>


class MillitaryUnit
{
private:
    QString unit_name;
    QVector <Conscript> conscripts;

public:
    MillitaryUnit(const QString& unit_name);

    QString get_unit_name() const;
    int get_conscript_count() const;
    void add_conscript(const Conscript& conscript);
    QVector<Conscript> get_conscripts() const;

};

#endif // MILLITARYUNIT_H
