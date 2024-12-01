#ifndef SKEET_ENGINE_TYPES_H
#define SKEET_ENGINE_TYPES_H

#include <cassert>
#include <cstdint>
#include <string>


using Key = uint64_t;
using Bitboard = uint64_t;

constexpr int MAX_MOVES = 256;
constexpr int MAX_PLY = 246;

enum Color {
    WHITE,
    BLACK,
    COLOR_NB = 2
};

enum CastlingRights {
    NO_CASTLING,
    WHITE_OO,
    WHITE_OOO = WHITE_OO << 1,
    BLACK_OO = WHITE_OO << 2,
    BLACK_OOO = WHITE_OO << 3,

    KING_SIDE = WHITE_OO | BLACK_OO,
    QUEEN_SIDE = WHITE_OOO | BLACK_OOO,
    WHITE_CASTLING = WHITE_OO | WHITE_OOO,
    BLACK_CASTLING = BLACK_OO | BLACK_OOO,
    ANY_CASTLING = WHITE_CASTLING | BLACK_CASTLING,

    CASTLING_RIGHT_NB = 16
};

// enum for transposition table node

/*enum Bound {
    BOUND_NONE,
    BOUND_UPPER,
    BOUND_LOWER,
    BOUND_EXACT = BOUND_UPPER | BOUND_LOWER
};*/

using Value = int;

constexpr Value VALUE_ZERO = 0;
constexpr Value VALUE_DRAW = 0;
constexpr Value VALUE_NONE = 32002;
constexpr Value VALUE_INFINITE = 32001;

constexpr Value VALUE_MATE = 32000;

constexpr Value PawnValue = 100;
constexpr Value KnightValue = 300;
constexpr Value BishopValue = 350; // seems like it does better with bishops than  with knights
constexpr Value RookValue = 500;
constexpr Value QueenValue = 900;

enum PieceType {
    NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    ALL_PIECES = 0,
    PIECE_TYPE_NB = 8
};

enum Piece {
    NO_PIECE,
    W_PAWN = PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    PIECE_NB = 16
};

constexpr Value PieceValue[PIECE_NB] = {
        VALUE_ZERO, PawnValue, KnightValue, BishopValue, RookValue, QueenValue, VALUE_ZERO, VALUE_ZERO,
        VALUE_ZERO, PawnValue, KnightValue, BishopValue, RookValue, QueenValue, VALUE_ZERO, VALUE_ZERO};



using Depth = int;

// has to be inverted vertically when used
const Value PiecePositionalValue[8][64] = {{}, // clion reformatting code )))
        // pawn
                                           {
                                                   0,   0,   0,   0,   0,   0,   0,   0,
                                                   50,  50,  50,  50,  50,  50,  50,  50,
                                                   10,  10,  20,  30,  30,  20,  10,  10,
                                                   5,   5,   10,  25,  25,  10,  5,   5,
                                                   0,   0,   0,   20,  20,  0,   0,   0,
                                                   5,   -5,  -10, 0,   0,   -10, -5,  5,
                                                   5,   10,  10, -20, -20, 10, 10,  5,
                                                   0,   0,   0,   0,   0,   0,   0,   0
                                           },
        // knight
                                           {
                                                   -50, -40, -30, -30, -30, -30, -40, -50,
                                                   -40, -20, 0,   0,   0,   0,   -20, -40,
                                                   -30, 0,   10,  15,  15,  10,  0,   -30,
                                                   -30, 5,   15,  20,  20,  15,  5,   -30,
                                                   -30, 0,   15,  20,  20,  15,  0,   -30,
                                                   -30, 5,   10,  15,  15,  10,  5,   -30,
                                                   -40, -20, 0,  5,   5,   0,  -20, -40,
                                                   -50, -40, -20, -30, -30, -20, -40, -50
                                           },
        // bishop
                                           {
                                                   -20, -10, -10, -10, -10, -10, -10, -20,
                                                   -10, 0,   0,   0,   0,   0,   0,   -10,
                                                   -10, 0,   5,   10,  10,  5,   0,   -10,
                                                   -10, 5,   5,   10,  10,  5,   5,   -10,
                                                   -10, 0,   10,  10,  10,  10,  0,   -10,
                                                   -10, 10,  10,  10,  10,  10,  10,  -10,
                                                   -10, 5,   0,  0,   0,   0,  5,   -10,
                                                   -20, -10, -40, -10, -10, -40, -10, -20
                                           },
        // rook
                                           {
                                                   0,   0,   0,   0,   0,   0,   0,   0,
                                                   5,   10,  10,  10,  10,  10,  10,  5,
                                                   -5,  0,   0,   0,   0,   0,   0,   -5,
                                                   -5,  0,   0,   0,   0,   0,   0,   -5,
                                                   -5,  0,   0,   0,   0,   0,   0,   -5,
                                                   -5,  0,   0,   0,   0,   0,   0,   -5,
                                                   -5,  0,   0,  0,   0,   0,  0,   -5,
                                                   0,   0,   0,   5,   5,   0,   0,   0
                                           },
        // queen
                                           {
                                                   -20, -10, -10, -5,  -5,  -10, -10, -20,
                                                   -10, 0,   0,   0,   0,   0,   0,   -10,
                                                   -10, 0,   5,   5,   5,   5,   0,   -10,
                                                   -5,  0,   5,   5,   5,   5,   0,   -5,
                                                   0,   0,   5,   5,   5,   5,   0,   -5,
                                                   -10, 5,   5,   5,   5,   5,   0,   -10,
                                                   -10, 0,   5,  0,   0,   0,  0,   -10,
                                                   -20, -10, -10, -5,  -5,  -10, -10, -20
                                           },
        // king
                                           {
                                                   -30, -40, -40, -50, -50, -40, -40, -30,
                                                   -30, -40, -40, -50, -50, -40, -40, -30,
                                                   -30, -40, -40, -50, -50, -40, -40, -30,
                                                   -30, -40, -40, -50, -50, -40, -40, -30,
                                                   -20, -30, -30, -40, -40, -30, -30, -20,
                                                   -10, -20, -20, -20, -20, -20, -20, -10,
                                                   20,  20,  0,  0,   0,   0,  20,  20,
                                                   20,  30,  10,  0,   0,   10,  30,  20
                                           },
                                           {
                                                   -50, -40, -30, -20, -20, -30, -40, -50,
                                                   -30, -20, -10, 0,   0,   -10, -20, -30,
                                                   -30, -10, 20,  30,  30,  20,  -10, -30,
                                                   -30, -10, 30,  40,  40,  30,  -10, -30,
                                                   -30, -10, 30,  40,  40,  30,  -10, -30,
                                                   -30, -10, 20,  30,  30,  20,  -10, -30,
                                                   -30, -30, 0,  0,   0,   0,  -30, -30,
                                                   -50, -30, -30, -30, -30, -30, -30, -50
                                           }
};



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
    SQUARE_NB = 64
};


enum Direction : int {
    NORTH = 8,
    EAST = 1,
    SOUTH = -NORTH,
    WEST = -EAST,

    NORTH_EAST = NORTH + EAST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST,
    NORTH_WEST = NORTH + WEST
};

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

#define ENABLE_INCR_OPERATORS_ON(T) \
        inline T& operator++(T& d) { return d = T(int(d) + 1); } \
        inline T& operator--(T& d) { return d = T(int(d) - 1); }

ENABLE_INCR_OPERATORS_ON(PieceType)

ENABLE_INCR_OPERATORS_ON(Square)

ENABLE_INCR_OPERATORS_ON(File)

ENABLE_INCR_OPERATORS_ON(Rank)

#undef ENABLE_INCR_OPERATORS_ON

constexpr Direction operator+(Direction d1, Direction d2) { return Direction(int(d1) + int(d2)); }

constexpr Direction operator*(int i, Direction d) { return Direction(i * int(d)); }

// Additional operators to add a Direction to a Square
constexpr Square operator+(Square s, Direction d) { return Square(int(s) + int(d)); }

constexpr Square operator-(Square s, Direction d) { return Square(int(s) - int(d)); }

inline Square &operator+=(Square &s, Direction d) { return s = s + d; }

inline Square &operator-=(Square &s, Direction d) { return s = s - d; }

// Toggle color
constexpr Color operator~(Color c) { return Color(c ^ BLACK); }

// Swap A1 <-> A8
constexpr Square flip_rank(Square s) { return Square(s ^ SQ_A8); }

// Swap A1 <-> H1
constexpr Square flip_file(Square s) { return Square(s ^ SQ_H1); }

// Swap color of piece B_KNIGHT <-> W_KNIGHT
constexpr Piece operator~(Piece pc) { return Piece(pc ^ 8); }

constexpr CastlingRights operator&(Color c, CastlingRights cr) {
    return CastlingRights((c == WHITE ? WHITE_CASTLING : BLACK_CASTLING) & cr);
}

constexpr Value mate_in(int ply) { return VALUE_MATE - ply; }

constexpr Value mated_in(int ply) { return -VALUE_MATE + ply; }

constexpr Square make_square(File f, Rank r) { return Square((r << 3) + f); }

constexpr Piece make_piece(Color c, PieceType pt) { return Piece((c << 3) + pt); }

constexpr PieceType type_of(Piece pc) { return PieceType(pc & 7); }

inline Color color_of(Piece pc) {
    assert(pc != NO_PIECE);
    return Color(pc >> 3);
}

constexpr bool is_ok(Square s) { return s >= SQ_A1 && s <= SQ_H8; }

constexpr File file_of(Square s) { return File(s & 7); }

constexpr Rank rank_of(Square s) { return Rank(s >> 3); }

constexpr Square relative_square(Color c, Square s) { return Square(s ^ (c * 56)); }

constexpr Rank relative_rank(Color c, Rank r) { return Rank(r ^ (c * 7)); }

constexpr Rank relative_rank(Color c, Square s) { return relative_rank(c, rank_of(s)); }

constexpr Direction pawn_push(Color c) { return c == WHITE ? NORTH : SOUTH; }


// Based on a congruential pseudo-random number generator
constexpr Key make_key(uint64_t seed) {
    return seed * 6364136223846793005ULL + 1442695040888963407ULL;
}


enum MoveType {
    NORMAL,
    PROMOTION = 1 << 14,
    EN_PASSANT = 2 << 14,
    CASTLING = 3 << 14
};

// A move needs 16 bits to be stored
//
// bit  0- 5: destination square (from 0 to 63)
// bit  6-11: origin square (from 0 to 63)
// bit 12-13: promotion piece type - 2 (from KNIGHT-2 to QUEEN-2)
// bit 14-15: special move flag: promotion (1), en passant (2), castling (3)
// Special cases are Move::none() and Move::null().
// in any normal move the destination square and origin square are always different,
// but Move::none() and Move::null() have the same origin and destination square.

class Move {
public:
    Move() = default;

    constexpr explicit Move(std::uint16_t d) :
            data(d) {}

    constexpr Move(Square from, Square to) :
            data((from << 6) + to) {}

    template<MoveType T>
    static constexpr Move make(Square from, Square to, PieceType pt = KNIGHT) {
        return Move(T + ((pt - KNIGHT) << 12) + (from << 6) + to);
    }

    [[nodiscard]] constexpr Square from_sq() const {
        assert(is_ok());
        return Square((data >> 6) & 0x3F);
    }

    [[nodiscard]] constexpr Square to_sq() const {
        assert(is_ok());
        return Square(data & 0x3F);
    }

    [[nodiscard]] constexpr int from_to() const { return data & 0xFFF; }

    [[nodiscard]] constexpr MoveType type_of() const { return MoveType(data & (3 << 14)); }

    [[nodiscard]] constexpr PieceType promotion_type() const { return PieceType(((data >> 12) & 3) + KNIGHT); }

    [[nodiscard]] constexpr bool is_ok() const { return none().data != data && null().data != data; }

    static constexpr Move null() { return Move(65); }

    static constexpr Move none() { return Move(0); }

    constexpr bool operator==(const Move &m) const { return data == m.data; }

    constexpr bool operator!=(const Move &m) const { return data != m.data; }

    constexpr explicit operator bool() const { return data != 0; }

    [[nodiscard]] constexpr std::uint16_t raw() const { return data; }

protected:
    std::uint16_t data;
};

const std::string squareNames[] = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "NONE"
};

inline std::string toStr(Move m) {
    if (m == Move::none())
        return "(none)";

    if (m == Move::null())
        return "0000";

    Square from = m.from_sq();
    Square to   = m.to_sq();

    if (m.type_of() == CASTLING)
        to = make_square(to > from ? FILE_G : FILE_C, rank_of(from));

    std::string move = squareNames[from] + squareNames[to];

    if (m.type_of() == PROMOTION)
        move += " pnbrqk"[m.promotion_type()];

    return move;

}


#endif //SKEET_ENGINE_TYPES_H
