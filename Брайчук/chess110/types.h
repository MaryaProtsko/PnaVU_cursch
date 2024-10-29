//
// Created by cs on 26.10.2024.
//

#ifndef CHESS110_TYPES_H
#define CHESS110_TYPES_H

#include <cassert>

namespace Chess{
    enum Square : int {
        SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
        SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
        SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
        SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
        SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
        SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
        SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
        SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
        SQ_NONE,

        SQUARE_ZERO = 0,
        SQUARE_NB   = 64
    }; // ?

    constexpr bool isSquare(Square s) { return s >= SQ_A1 && s <= SQ_H8; }





    enum PieceType {
        NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
        ALL_PIECES = 0,
        PIECE_TYPE_NB = 8
    };

    enum Color {
        WHITE,
        BLACK,
        COLOR_NB = 2
    };

    constexpr  Color operator~(Color c) {return Color(c ^ BLACK);}

    constexpr Square relativeSquare(Color c, Square s) { return Square(s ^ (c * 56)); }

    enum MoveType {
        NORMAL,
        PROMOTION  = 1 << 14,
        EN_PASSANT = 2 << 14,
        CASTLING   = 3 << 14
    };

    enum Direction : int {
        NORTH = 8,
        EAST  = 1,
        SOUTH = -NORTH,
        WEST  = -EAST,

        NORTH_EAST = NORTH + EAST,
        SOUTH_EAST = SOUTH + EAST,
        SOUTH_WEST = SOUTH + WEST,
        NORTH_WEST = NORTH + WEST
    };

    using Offset = int;

    constexpr Square operator+(Square s, Offset d) { return Square(int(s) + int(d)); }
    constexpr Square operator-(Square s, Offset d) { return Square(int(s) - int(d)); }
    inline Square& operator+=(Square& s, Offset d) { return s = s + d; }
    inline Square& operator-=(Square& s, Offset d) { return s = s - d; }

    enum File : int {
        FILE_A,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H,
        FILE_NB
    };

    enum Rank : int {
        RANK_1,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8,
        RANK_NB
    };

    constexpr Square makeSquare(File f, Rank r) { return Square((r << 3) + f); }

    constexpr File fileOf(Square s) { return File(s & 7); }

    constexpr Rank rankOf(Square s) { return Rank(s >> 3); }

    constexpr Rank relativeRank(Color c, Rank r) { return Rank(r ^ (c * 7)); }

    constexpr Rank relativeRank(Color c, Square s) { return relativeRank(c, rankOf(s)); }

    enum CastlingRights {
        NO_CASTLING,
        WHITE_OO,
        WHITE_OOO = WHITE_OO << 1,
        BLACK_OO  = WHITE_OO << 2,
        BLACK_OOO = WHITE_OO << 3,

        KING_SIDE      = WHITE_OO | BLACK_OO,
        QUEEN_SIDE     = WHITE_OOO | BLACK_OOO,
        WHITE_CASTLING = WHITE_OO | WHITE_OOO,
        BLACK_CASTLING = BLACK_OO | BLACK_OOO,
        ANY_CASTLING   = WHITE_CASTLING | BLACK_CASTLING,

        CASTLING_RIGHT_NB = 16
    };

    constexpr CastlingRights operator&(Color c, CastlingRights cr) {
        return CastlingRights((c == WHITE ? WHITE_CASTLING : BLACK_CASTLING) & cr);
    }



}


#endif //CHESS110_TYPES_H
