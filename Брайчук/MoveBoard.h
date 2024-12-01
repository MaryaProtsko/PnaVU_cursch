#ifndef SKEET_ENGINE_MOVEBOARD_H
#define SKEET_ENGINE_MOVEBOARD_H

#include "ZorbistFenBoard.h"


// Does moves
class MoveBoard : public ZorbistFenBoard {

public:
    // Move properties
    bool legal(Move m) const;

    bool pseudo_legal(const Move m) const;

    bool capture(Move m) const;

    bool capture_stage(Move m) const;

    bool gives_check(Move m) const;

    Piece moved_piece(Move m) const;



    Key key_after(Move m) const;

    // Doing and undoing moves
    void do_move(Move m);

    void do_move(Move m, bool givesCheck);

    void undo_move(Move m);

    void do_null_move();

    void undo_null_move();

    // Static Exchange Evaluation
    bool see_ge(Move m, int threshold = 0) const;

private:
    template<bool Do>
    void do_castling(Color us, Square from, Square &to, Square &rfrom, Square &rto);
};

inline Piece MoveBoard::moved_piece(Move m) const { return piece_on(m.from_sq()); }

inline bool MoveBoard::capture(Move m) const {
    assert(m.is_ok());
    return (!empty(m.to_sq()) && m.type_of() != CASTLING) || m.type_of() == EN_PASSANT;
}

inline bool MoveBoard::capture_stage(Move m) const {
    assert(m.is_ok());
    return capture(m) || m.promotion_type() == QUEEN;
}



inline void MoveBoard::do_move(Move m) { do_move(m, gives_check(m)); }


#endif //SKEET_ENGINE_MOVEBOARD_H
