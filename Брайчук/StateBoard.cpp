#include "StateBoard.h"

// Tests whether the position is drawn by 50-move rule
// or by repetition. It does not detect stalemates.
bool StateBoard::is_draw(int ply) const {

    if (curSt.rule50 > 99)
        return true;

    // Return a draw score if a position repeats once earlier but strictly
    // after the root, or repeats twice before or at the root. In case of
    // three-fold repetition the value would be negative
    return curSt.repetition && curSt.repetition < ply;
}


// Tests whether there has been at least one repetition
// of positions since the last capture or pawn move.
bool StateBoard::has_repeated() {

    int end = std::min(curSt.rule50, curSt.pliesFromNull);

    if (curSt.repetition) return true;

    auto stc = previousStates.end() - 2;

    while (end-- >= 3)
    {
        if (stc->repetition)
            return true;

        stc--;
    }
    return false;
}

