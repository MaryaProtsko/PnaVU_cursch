#ifndef SKEET_ENGINE_PieceBoard_H
#define SKEET_ENGINE_PieceBoard_H

#include <cassert>
#include <deque>
#include <iosfwd>
#include <memory>
#include <string>
#include <cstring>

#include "bitboard.h"

#include "types.h"


// Stores pieces on the board in different
// representations
class PieceBoard {

public:
    PieceBoard() = default;

    PieceBoard(const PieceBoard &) = delete;

    PieceBoard &operator=(const PieceBoard &) = delete;

    // Board representation
    Bitboard pieces(PieceType pt = ALL_PIECES) const;

    template<typename... PieceTypes>
    Bitboard pieces(PieceType pt, PieceTypes... pts) const;

    Bitboard pieces(Color c) const;

    template<typename... PieceTypes>
    Bitboard pieces(Color c, PieceTypes... pts) const;

    Piece piece_on(Square s) const;

    bool empty(Square s) const;

    template<PieceType Pt>
    int count(Color c) const;

    template<PieceType Pt>
    int count() const;

    template<PieceType Pt>
    Square square(Color c) const;

    void put_piece(Piece pc, Square s);

    void remove_piece(Square s);

    void move_piece(Square from, Square to);



protected:
    void reset();
    // Data members
    Piece board[SQUARE_NB];
    Bitboard byTypeBB[PIECE_TYPE_NB];
    Bitboard byColorBB[COLOR_NB];
    int pieceCount[PIECE_NB]; // keep track of the count incrementally
};

inline Piece PieceBoard::piece_on(Square s) const {
    assert(is_ok(s));
    return board[s];
}

inline bool PieceBoard::empty(Square s) const { return piece_on(s) == NO_PIECE; }

inline Bitboard PieceBoard::pieces(PieceType pt) const { return byTypeBB[pt]; }

template<typename... PieceTypes>
inline Bitboard PieceBoard::pieces(PieceType pt, PieceTypes... pts) const {
    return pieces(pt) | pieces(pts...);
}

inline Bitboard PieceBoard::pieces(Color c) const { return byColorBB[c]; }

template<typename... PieceTypes>
inline Bitboard PieceBoard::pieces(Color c, PieceTypes... pts) const {
    return pieces(c) & pieces(pts...);
}

template<PieceType Pt>
inline int PieceBoard::count(Color c) const {
    return pieceCount[make_piece(c, Pt)];
}

template<PieceType Pt>
inline int PieceBoard::count() const {
    return count<Pt>(WHITE) + count<Pt>(BLACK);
}

template<PieceType Pt>
inline Square PieceBoard::square(Color c) const {
    assert(count<Pt>(c) == 1);
    return lsb(pieces(c, Pt));
}

inline void PieceBoard::put_piece(Piece pc, Square s) {
    board[s] = pc;
    byTypeBB[ALL_PIECES] |= byTypeBB[type_of(pc)] |= s;
    byColorBB[color_of(pc)] |= s;
    pieceCount[pc]++;
    pieceCount[make_piece(color_of(pc), ALL_PIECES)]++;
}

inline void PieceBoard::remove_piece(Square s) {
    Piece pc = board[s];
    byTypeBB[ALL_PIECES] ^= s;
    byTypeBB[type_of(pc)] ^= s;
    byColorBB[color_of(pc)] ^= s;
    board[s] = NO_PIECE;
    pieceCount[pc]--;
    pieceCount[make_piece(color_of(pc), ALL_PIECES)]--;
}

inline void PieceBoard::move_piece(Square from, Square to) {
    Piece pc = board[from];
    Bitboard fromTo = from | to;
    byTypeBB[ALL_PIECES] ^= fromTo;
    byTypeBB[type_of(pc)] ^= fromTo;
    byColorBB[color_of(pc)] ^= fromTo;
    board[from] = NO_PIECE;
    board[to] = pc;
}

inline void PieceBoard::reset() {
    memset(this, 0, sizeof(PieceBoard));
}


#endif //SKEET_ENGINE_PieceBoard_H
