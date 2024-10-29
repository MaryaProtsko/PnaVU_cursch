//
// Created by cs on 25.10.2024.
//

#ifndef CHESS110_SIMPLEBOARD_H
#define CHESS110_SIMPLEBOARD_H

#include <vector>
#include <algorithm>
#include "types.h"
#include "Piece.h"

namespace Chess {

class SimpleBoard {



public:
    void addPiece(Square sq, Piece pc);
    void deletePiece(Square sq);
    void movePiece(Square sq, Square to);
    Piece pieceAt(Square sq) const;
    bool isEmpty(Square sq) const;
    std::vector<Square>& piecesOfColor(Color c);

private:
    Piece squares[SQUARE_NB];
    std::vector<Square> pieceSquares[COLOR_NB];

};


}

#endif //CHESS110_SIMPLEBOARD_H
