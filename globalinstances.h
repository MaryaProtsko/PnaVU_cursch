#ifndef GLOBALINSTANCES_H
#define GLOBALINSTANCES_H

#include "components/logger.h"
#include "components/database/database.h"

struct InstancesPack{
    Logger* logger;
    Database* database;
};
enum Instances{ LOGGER, DATABASE };

class GlobalInstances {
private:
    static Logger* logger;
    static Database* database;
public:
    GlobalInstances(struct InstancesPack ip);
    static void delete_instances();
    static void* getInstance(enum Instances i);
    static Database* getDatabase();
    static Logger* getLogger();
};

#endif // GLOBALINSTANCES_H
