#ifndef SKEET_ENGINE_ZORBISTFENBOARD_H
#define SKEET_ENGINE_ZORBISTFENBOARD_H

#include "PieceBoard.h"
#include "StateBoard.h"

extern Square castlingRookSquare[CASTLING_RIGHT_NB];
extern Bitboard castlingPath[CASTLING_RIGHT_NB];
extern int castlingRightsMask[SQUARE_NB];

namespace Zobrist {
    extern Key psq[PIECE_NB][SQUARE_NB];
    extern Key enpassant[FILE_NB];
    extern Key castling[CASTLING_RIGHT_NB];
    extern Key side;
}

class ZorbistFenBoard : public PieceBoard, public StateBoard {
public:
    static void init();

    ZorbistFenBoard() = default;

    // FEN string input/output
    ZorbistFenBoard&   set(const std::string& fenStr);
    std::string fen() const;


    // Castling
    bool           castling_impeded(CastlingRights cr) const;
    Square         castling_rook_square(CastlingRights cr) const;

    // Attacks to/from a given square
    Bitboard attackers_to(Square s) const;
    Bitboard attackers_to(Square s, Bitboard occupied) const;
    void     update_slider_blockers(Color c);
    template<PieceType Pt>
    Bitboard attacks_by(Color c) const;

    // positional
    int board_phase() const;
    Value positional_value_of(Piece pc, Square s) const;

    //Board consistency check, for debugging
    bool pos_is_ok() const;
    void flip();


protected:
    void set_state();
    void set_check_info();
};

std::ostream& operator<<(std::ostream& os, const ZorbistFenBoard& pos);

inline bool ZorbistFenBoard::castling_impeded(CastlingRights cr) const {
    assert(cr == WHITE_OO || cr == WHITE_OOO || cr == BLACK_OO || cr == BLACK_OOO);
    return pieces() & castlingPath[cr];
}

inline Square ZorbistFenBoard::castling_rook_square(CastlingRights cr) const {
    assert(cr == WHITE_OO || cr == WHITE_OOO || cr == BLACK_OO || cr == BLACK_OOO);
    return castlingRookSquare[cr];

}

inline Bitboard ZorbistFenBoard::attackers_to(Square s) const { return attackers_to(s, pieces()); }

template<PieceType Pt>
inline Bitboard ZorbistFenBoard::attacks_by(Color c) const {

    if constexpr (Pt == PAWN)
        return c == WHITE ? pawn_attacks_bb<WHITE>(pieces(WHITE, PAWN))
                          : pawn_attacks_bb<BLACK>(pieces(BLACK, PAWN));
    else
    {
        Bitboard threats   = 0;
        Bitboard attackers = pieces(c, Pt);
        while (attackers)
            threats |= attacks_bb<Pt>(pop_lsb(attackers), pieces());
        return threats;
    }
}
#endif //SKEET_ENGINE_ZORBISTFENBOARD_H
