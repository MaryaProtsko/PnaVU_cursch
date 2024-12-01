#ifndef SKEET_ENGINE_UCI_H
#define SKEET_ENGINE_UCI_H

#include <thread>
#include "MoveBoard.h"
#include "Search.h"
#include <chrono>

#define sync_cout std::cout << IO_LOCK
#define sync_endl std::endl << IO_UNLOCK

class UCI {
    MoveBoard b;
    Search search;
    //SearchInfo info;
    std::thread thr;

    void find_move();
public:
    UCI();
    void loop();
    static void print_info(SearchInfo info);

    Move string_to_move(std::string& s);

};



#endif //SKEET_ENGINE_UCI_H
