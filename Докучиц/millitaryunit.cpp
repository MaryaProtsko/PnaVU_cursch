//реализация основных функций класса
#include "millitaryunit.h"


MillitaryUnit::MillitaryUnit(const QString& unit_name) : unit_name(unit_name) {}


QString MillitaryUnit::get_unit_name() const
{
    return unit_name;
}

int MillitaryUnit::get_conscript_count() const
{
    return conscripts.size();
}

void MillitaryUnit::add_conscript(const Conscript& conscript) {
    conscripts.append(conscript);
}

QVector<Conscript> MillitaryUnit::get_conscripts() const {
    return conscripts;
}
