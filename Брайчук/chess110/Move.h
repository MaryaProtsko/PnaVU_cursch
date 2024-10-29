//
// Created by cs on 25.10.2024.
//

#ifndef CHESS110_MOVE_H
#define CHESS110_MOVE_H
#include <cstdint>
#include "types.h"

namespace Chess {


    class Move {
    public:
         Move();

         explicit Move(uint16_t d);

        Move(Square from, Square to);

        Move(Square from, Square to, MoveType mt, PieceType pt = KNIGHT);


        Square from() const;

        Square to() const;

        MoveType moveType() const;

        PieceType promotionType() const;

        bool isMove() const;

        static Move null();
        static Move none();

        bool operator==(const Move& m) const;
        bool operator!=(const Move& m) const;

        explicit operator bool() const;

    private:
        std::uint16_t move;
    };
}


#endif //CHESS110_MOVE_H
