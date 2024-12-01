
#include <iostream>
#include "Eval.h"
#include "bitboard.h"



Value Eval::eval() {
    Value value = 0;

    value += b.material_value(WHITE) - b.material_value(BLACK);

    value += b.positional_value(WHITE) - b.positional_value(BLACK);

    // TODO pawns fast check
    // for now eval does not check due to ineffective time consume

    return b.sideToMove == WHITE ? value : -value;
}