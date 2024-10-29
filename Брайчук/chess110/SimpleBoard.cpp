//
// Created by cs on 25.10.2024.
//

#include "SimpleBoard.h"


namespace Chess {
    void SimpleBoard::addPiece(Chess::Square sq, Chess::Piece pc) {
        pieceSquares[pc.color()].push_back(sq);
        squares[sq] = pc;
    }
    void SimpleBoard::deletePiece(Square sq){
        Piece p = squares[sq];
        Color c = p.color();
        pieceSquares[c].erase(std::find(pieceSquares[c].begin(), pieceSquares[c].end(),sq));
        squares[sq];
    }
    void SimpleBoard::movePiece(Square fr, Square to){
        Piece p = squares[fr];
        Color c = p.color();
        squares[to] = squares[fr];
        squares[fr] = Piece(NO_PIECE_TYPE);
        pieceSquares[c].erase(std::find(pieceSquares[c].begin(), pieceSquares[c].end(),fr));
        pieceSquares[c].push_back(to);
    }

    std::vector<Square>& SimpleBoard::piecesOfColor(Color c){
        return pieceSquares[c];
    }

    Piece SimpleBoard::pieceAt(Square sq) const {
        return squares[sq];
    }

    bool SimpleBoard::isEmpty(Chess::Square sq) const {
        return !(bool(squares[sq]));
    }
}