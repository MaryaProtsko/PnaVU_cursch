#include "globalinstances.h"

Logger* GlobalInstances::logger = nullptr;
Database* GlobalInstances::database = nullptr;

GlobalInstances::GlobalInstances(struct InstancesPack ip) {
    logger = ip.logger;
    database = ip.database;
}
void GlobalInstances::delete_instances(){
    delete logger;
    delete database;
}
void* GlobalInstances::getInstance(enum Instances i){
    switch(i){
    case LOGGER:
        return getLogger(); break;
    case DATABASE:
        return getDatabase(); break;
    }
    return nullptr;
}
Database* GlobalInstances::getDatabase(){
    return database;
}
Logger* GlobalInstances::getLogger(){
    return logger;
}
