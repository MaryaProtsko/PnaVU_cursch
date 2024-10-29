//
// Created by cs on 25.10.2024.
//

#include "Piece.h"


namespace Chess{
    Piece::Piece():piece(NO_PIECE_TYPE){}
     Piece::Piece(int p): piece(p){}
    Piece::Piece(Color c, PieceType pt):piece(pt + (c << 3)){}
    Piece Piece::operator~() const{
        return Piece(piece ^ 8);
    }
    PieceType Piece::pieceType() const {
        return PieceType(piece & 7);
    }
    Color Piece::color() const {
        //assert(pieceType() != NO_PIECE_TYPE);
        return Color(piece >> 3);
    }
    Piece::operator bool() const {
        return pieceType() != NO_PIECE_TYPE;
    }

    uint8_t Piece::raw(){
        return piece;
    }

}