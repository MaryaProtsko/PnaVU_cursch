//
// Created by cs on 25.10.2024.
//

#include "Move.h"

namespace Chess {
    Move::Move() = default;

    Move:: Move(uint16_t d) : move(d) {}

    Move::Move(Square from, Square to) : move((from << 6) + to) {}

    Move::Move(Square from, Square to, MoveType mt, PieceType pt) : move(mt + ((pt - KNIGHT) << 12) + (from << 6) + to) {}


    Square Move::from() const {
        assert(isMove());
        return Square((move >> 6) & 0x3F);
    }

    Square Move::to() const {
        assert(isMove());
        return Square(move & 0x3F);
    }

    MoveType Move::moveType() const { return MoveType(move & (3 << 14)); }

    PieceType Move::promotionType() const { return PieceType(((move >> 12) & 3) + KNIGHT); }

    bool Move::isMove() const { return none().move != move && null().move != move; }

    Move Move::null() { return Move(65); }
    Move Move::none() { return Move(0); }

    bool Move::operator==(const Move& m) const { return move == m.move; }
    bool Move::operator!=(const Move& m) const { return move != m.move; }

    Move::operator bool() const { return move != 0; }
}