#ifndef SKEET_ENGINE_EVAL_H
#define SKEET_ENGINE_EVAL_H

#include "MoveBoard.h"

class Eval {
    public:

    Eval(MoveBoard& bd):b(bd){};
    Value eval();

    private:
    MoveBoard& b;
};


#endif //SKEET_ENGINE_EVAL_H
