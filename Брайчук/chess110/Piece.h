//
// Created by cs on 25.10.2024.
//

#ifndef CHESS110_PIECE_H
#define CHESS110_PIECE_H


#include <cstdint>
#include "types.h"

namespace Chess {

    class Piece {
    public:
        Piece();
        explicit Piece(int p);
        Piece(Color c, PieceType pt);
        Piece operator~() const;
        PieceType pieceType() const;
        Color color() const;
        uint8_t raw();
        explicit operator bool() const;
        constexpr bool operator==(const Piece& m) const { return piece == m.piece; }
        constexpr bool operator!=(const Piece& m) const { return piece != m.piece; }
    private:
        uint8_t piece;
    };

    // Keep track of what a move changes on the board (used by NNUE and graphics)
    struct DirtyPiece {

        // Number of changed pieces
        int dirty_num;

        // Max 3 pieces can change in one move. A promotion with capture moves
        // both the pawn and the captured piece to SQ_NONE and the piece promoted
        // to from SQ_NONE to the capture square.
        Piece piece[3];

        // From and to squares, which may be SQ_NONE
        Square from[3];
        Square to[3];
    };

}


#endif //CHESS110_PIECE_H
