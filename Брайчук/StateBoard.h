
#ifndef SKEET_ENGINE_STATEBOARD_H
#define SKEET_ENGINE_STATEBOARD_H


#include <cassert>
#include <iosfwd>
#include <memory>
#include <string>
#include <deque>
#include <cstring>

#include "bitboard.h"
#include "types.h"


// DirtyState struct does store information needed to restore board object to
// its previous state when we retract a move.

struct DirtyState {

    // Copied when making a move
    Value materialValue[COLOR_NB];
    Value positionalValue[COLOR_NB];
    Value kingLastValue[COLOR_NB];
    int castlingRights;
    int rule50;
    int pliesFromNull;
    Square epSquare;

    // Recalculated when making a move
    Key key;
    Bitboard checkersBB;
    Bitboard blockersForKing[COLOR_NB];
    Bitboard pinners[COLOR_NB];
    Bitboard checkSquares[PIECE_TYPE_NB];
    Piece capturedPiece;
    int repetition;
    constexpr bool operator==(DirtyState& d2) const;
};

constexpr bool DirtyState::operator==(DirtyState &d2) const{
    return d2.key == key;
}


// StateBoard class stores information regarding the board representation as
// side to move, hash keys, castling info, etc.
class StateBoard {
public:

    StateBoard() = default;

    Square ep_square() const;

    // Castling
    CastlingRights castling_rights(Color c) const;

    bool can_castle(CastlingRights cr) const;

    // Checking
    Bitboard checkers() const;

    Bitboard blockers_for_king(Color c) const;

    Bitboard check_squares(PieceType pt) const;

    Bitboard pinners(Color c) const;


    // Evaluation info

    Value material_value() const;

    Value material_value(Color c) const;

    Value positional_value() const;

    Value positional_value(Color c) const;


    // Accessing hash keys
    Key key() const;

    template<bool AfterMove>
    Key adjust_key50(Key k) const;

    // Other properties of the StateBoard
    Color side_to_move() const;

    int game_ply() const;

    int rule50_count() const;

    Piece captured_piece() const;

    bool  is_draw(int ply) const;
    bool  has_repeated();

    const DirtyState &state() const;



// for debug
public: //TODO

    void reset();
    // Data members

    std::deque<DirtyState> previousStates;
    DirtyState curSt;
    int gamePly;
    Color sideToMove;
};

inline Color StateBoard::side_to_move() const { return sideToMove; }


inline Square StateBoard::ep_square() const { return curSt.epSquare; }

inline bool StateBoard::can_castle(CastlingRights cr) const { return curSt.castlingRights & cr; }

inline CastlingRights StateBoard::castling_rights(Color c) const {
    return c & CastlingRights(curSt.castlingRights);
}


inline Bitboard StateBoard::checkers() const { return curSt.checkersBB; }

inline Bitboard StateBoard::blockers_for_king(Color c) const { return curSt.blockersForKing[c]; }

inline Bitboard StateBoard::pinners(Color c) const { return curSt.pinners[c]; }

inline Bitboard StateBoard::check_squares(PieceType pt) const { return curSt.checkSquares[pt]; }

inline Key StateBoard::key() const {
    return curSt.key;
    //return adjust_key50<false>(curSt.key);
}

template<bool AfterMove>
inline Key StateBoard::adjust_key50(Key k) const {
    return curSt.rule50 < 14 - AfterMove ? k : k ^ make_key((curSt.rule50 - (14 - AfterMove)) / 8);
}

inline Value StateBoard::material_value(Color c) const { return curSt.materialValue[c]; }

inline Value StateBoard::material_value() const {
    return material_value(WHITE) + material_value(BLACK);
}

inline Value StateBoard::positional_value(Color c) const { return curSt.positionalValue[c]; }

inline Value StateBoard::positional_value() const {
    return positional_value(WHITE) + positional_value(BLACK);
}

inline int StateBoard::game_ply() const { return gamePly; }

inline int StateBoard::rule50_count() const { return curSt.rule50; }

inline Piece StateBoard::captured_piece() const { return curSt.capturedPiece; }


inline const DirtyState &StateBoard::state() const { return curSt; }

inline void StateBoard::reset() {
    previousStates.clear();

    std::memset(&curSt, 0, sizeof(DirtyState));
}



#endif //SKEET_ENGINE_STATEBOARD_H
