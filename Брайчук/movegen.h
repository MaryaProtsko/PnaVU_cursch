#ifndef SKEET_ENGINE_MOVEGEN_H
#define SKEET_ENGINE_MOVEGEN_H

#include <algorithm>
#include <cstddef>

#include "types.h"


class MoveBoard;

enum GenType {
    CAPTURES,
    QUIETS,
    EVASIONS,
    NON_EVASIONS,
    LEGAL
};

struct ExtMove : public Move {
    int value;

    void operator=(Move m) { data = m.raw(); }

    //ExtMove(Move m, int val):Move(m),value(val){} // TODO

    // )))))))))))))))))))))))))))))))))
    operator float() const = delete;
};

inline bool operator<(const ExtMove &f, const ExtMove &s) { return f.value < s.value; }
inline bool operator>(const ExtMove &f, const ExtMove &s) { return f.value > s.value; }

template<GenType>
ExtMove *generate(const MoveBoard &pos, ExtMove *moveList);

// The MoveList struct wraps the generate() function and returns a convenient
// list of moves. TODO sometimes use generate
template<GenType T>
struct MoveList {

    explicit MoveList(const MoveBoard &pos) :
            last(generate<T>(pos, moveList)) {}

    const ExtMove *begin() const { return moveList; }

    const ExtMove *end() const { return last; }

    size_t size() const { return last - moveList; }

    bool contains(Move move) const { return std::find(begin(), end(), move) != end(); }

private:
    ExtMove moveList[MAX_MOVES], *last;
};


#endif //SKEET_ENGINE_MOVEGEN_H
