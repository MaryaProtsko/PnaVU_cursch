#include "movegen.h"

#include <cassert>
#include "bitboard.h"
#include "MoveBoard.h"


namespace {
    // Enemy determines if there is an enemy on promotion sq
    template<GenType Type, Direction D, bool Enemy>
    ExtMove *make_promotions(ExtMove *moveList, Square to) {

        constexpr bool all = Type == EVASIONS || Type == NON_EVASIONS;

        if constexpr (Type == CAPTURES || all) // promoting to queen is considered capture
            *moveList++ = Move::make<PROMOTION>(to - D, to, QUEEN);

        if constexpr ((Type == CAPTURES && Enemy) || (Type == QUIETS && !Enemy) || all) {
            *moveList++ = Move::make<PROMOTION>(to - D, to, ROOK);
            *moveList++ = Move::make<PROMOTION>(to - D, to, BISHOP);
            *moveList++ = Move::make<PROMOTION>(to - D, to, KNIGHT);
        }

        return moveList;
    }

    // pawn moves are generated in a separate function due
    // the following reasons
    // 1. attacks_bb can't be used due to pawn style of moving
    // 2. promotions
    // 3. en passant
    // 4. usually there are many pawns, and iterating over them costs much,
    // so we just shift all pawns together
    template<Color Us, GenType Type>
    ExtMove *generate_pawn_moves(const MoveBoard &pos, ExtMove *moveList, Bitboard target) {


        // color consistent variables
        constexpr Color Them = ~Us;
        constexpr Bitboard TRank7BB = (Us == WHITE ? Rank7BB : Rank2BB); // for promotions
        constexpr Bitboard TRank3BB = (Us == WHITE ? Rank3BB : Rank6BB); // used for double pawn moves
        constexpr Direction Up = pawn_push(Us);
        constexpr Direction UpRight = (Us == WHITE ? NORTH_EAST : SOUTH_WEST);
        constexpr Direction UpLeft = (Us == WHITE ? NORTH_WEST : SOUTH_EAST);


        const Bitboard emptySquares = ~pos.pieces();

        // in check we can capture only a checker
        // we cant block by capturing, otherwise the captured piece
        // would be blocking the check
        const Bitboard enemies = Type == EVASIONS ? pos.checkers() : pos.pieces(Them);

        Bitboard pawnsOn7 = pos.pieces(Us, PAWN) & TRank7BB; // these can only promote
        Bitboard pawnsNotOn7 = pos.pieces(Us, PAWN) & ~TRank7BB;

        // Single and double pawn pushes, no promotions
        if constexpr (Type != CAPTURES) {
            // moving forward only to an empty sq
            Bitboard b1 = shift<Up>(pawnsNotOn7) & emptySquares;

            // the third rank pawns are here (at b1) already on the forth
            Bitboard b2 = shift<Up>(b1 & TRank3BB) & emptySquares;

            if constexpr (Type == EVASIONS)  // Consider only blocking squares
            {
                b1 &= target;
                b2 &= target;
            }

            while (b1) {
                Square to = pop_lsb(b1);
                *moveList++ = Move(to - Up, to);
            }

            while (b2) {
                Square to = pop_lsb(b2);
                *moveList++ = Move(to - Up - Up, to);
            }
        }

        // Promotions and underpromotions
        if (pawnsOn7) {
            Bitboard b1 = shift<UpRight>(pawnsOn7) & enemies;
            Bitboard b2 = shift<UpLeft>(pawnsOn7) & enemies;
            Bitboard b3 = shift<Up>(pawnsOn7) & emptySquares;

            if constexpr (Type == EVASIONS)
                b3 &= target;

            while (b1)
                moveList = make_promotions<Type, UpRight, true>(moveList, pop_lsb(b1));

            while (b2)
                moveList = make_promotions<Type, UpLeft, true>(moveList, pop_lsb(b2));

            while (b3)
                moveList = make_promotions<Type, Up, false>(moveList, pop_lsb(b3));
        }

        // Standard and en passant captures
        if constexpr (Type == CAPTURES || Type == EVASIONS || Type == NON_EVASIONS) {

            // dioganally only capturing
            Bitboard b1 = shift<UpRight>(pawnsNotOn7) & enemies;
            Bitboard b2 = shift<UpLeft>(pawnsNotOn7) & enemies;

            while (b1) {
                Square to = pop_lsb(b1);
                *moveList++ = Move(to - UpRight, to);
            }

            while (b2) {
                Square to = pop_lsb(b2);
                *moveList++ = Move(to - UpLeft, to);
            }

            if (pos.ep_square() != SQ_NONE) {
                assert(rank_of(pos.ep_square()) == relative_rank(Us, RANK_6));

                // An en passant capture cannot resolve a discovered check
                if (Type == EVASIONS && (target & (pos.ep_square() + Up)))
                    return moveList;

                b1 = pawnsNotOn7 & pawn_attacks_bb(Them, pos.ep_square());

                assert(b1); // ep_square has to be initiated only when there
                            // is a pawn to invoke an en passant

                while (b1)
                    *moveList++ = Move::make<EN_PASSANT>(pop_lsb(b1), pos.ep_square());
            }
        }

        return moveList;
    }


    template<Color Us, PieceType Pt>
    ExtMove *generate_moves(const MoveBoard &pos, ExtMove *moveList, Bitboard target) {

        static_assert(Pt != KING && Pt != PAWN, "Unsupported piece type in generate_moves()");

        Bitboard bb = pos.pieces(Us, Pt);

        while (bb) {
            Square from = pop_lsb(bb);
            Bitboard b = attacks_bb<Pt>(from, pos.pieces()) & target;

            while (b)
                *moveList++ = Move(from, pop_lsb(b));
        }

        return moveList;
    }


    template<Color Us, GenType Type>
    ExtMove *generate_all(const MoveBoard &pos, ExtMove *moveList) {

        static_assert(Type != LEGAL, "Unsupported type in generate_all()");

        const Square ksq = pos.square<KING>(Us);
        Bitboard target;

        // Skip generating non-king moves when in double check
        if (Type != EVASIONS || !more_than_one(pos.checkers())) {
            // target includes checking piece and squares between the king
            // and the checker (blocking squares) in case of EVASIONS
            // empty squares combined with enemy squares (for captures) in case of NON_EVASIONS
            // empty squares in case of QUIETS
            // enemy squares in case of CAPTURES
            target = Type == EVASIONS ? between_bb(ksq, lsb(pos.checkers()))
                                      : Type == NON_EVASIONS ? ~pos.pieces(Us)
                                                             : Type == CAPTURES ? pos.pieces(~Us)
                                                                                : ~pos.pieces();  // QUIETS

            moveList = generate_pawn_moves<Us, Type>(pos, moveList, target);
            moveList = generate_moves<Us, KNIGHT>(pos, moveList, target);
            moveList = generate_moves<Us, BISHOP>(pos, moveList, target);
            moveList = generate_moves<Us, ROOK>(pos, moveList, target);
            moveList = generate_moves<Us, QUEEN>(pos, moveList, target);
        }

        // for the king when EVASIONS target contains
        // squares the king can't move to
        Bitboard b = attacks_bb<KING>(ksq) & (Type == EVASIONS ? ~pos.pieces(Us) : target);

        // the availability of the squares will be checked later;
        while (b)
            *moveList++ = Move(ksq, pop_lsb(b));

        // castling is encoded as king takes friendly rook
        if ((Type == QUIETS || Type == NON_EVASIONS) && pos.can_castle(Us & ANY_CASTLING))
            for (CastlingRights cr: {Us & KING_SIDE, Us & QUEEN_SIDE}) {
                if (!pos.castling_impeded(cr) && pos.can_castle(cr)) {
                    *moveList++ = Move::make<CASTLING>(ksq, pos.castling_rook_square(cr));

                }
            }

        return moveList;
    }

}  // namespace


// <CAPTURES>     Generates all pseudo-legal captures plus queen promotions
// <QUIETS>       Generates all pseudo-legal non-captures and underpromotions
// <EVASIONS>     Generates all pseudo-legal check evasions
// <NON_EVASIONS> Generates all pseudo-legal captures and non-captures
//
// Returns a pointer to the end of the move list.
template<GenType Type>
ExtMove *generate(const MoveBoard &pos, ExtMove *moveList) {

    static_assert(Type != LEGAL, "Unsupported type in generate()");
    assert((Type == EVASIONS) == bool(pos.checkers()));

    Color us = pos.side_to_move();

    return us == WHITE ? generate_all<WHITE, Type>(pos, moveList)
                       : generate_all<BLACK, Type>(pos, moveList);
}

// Explicit template instantiations

// this functions return pseudo legal moves

template ExtMove *generate<CAPTURES>(const MoveBoard &, ExtMove *);

template ExtMove *generate<QUIETS>(const MoveBoard &, ExtMove *);

template ExtMove *generate<EVASIONS>(const MoveBoard &, ExtMove *);

template ExtMove *generate<NON_EVASIONS>(const MoveBoard &, ExtMove *);


// generate<LEGAL> generates all the legal moves in the given MoveBoard

template<>
ExtMove *generate<LEGAL>(const MoveBoard &pos, ExtMove *moveList) {

    Color us = pos.side_to_move();

    // blockers contains also enemy pieces
    Bitboard pinned = pos.blockers_for_king(us) & pos.pieces(us);
    Square ksq = pos.square<KING>(us);
    ExtMove *cur = moveList;

    moveList =
            pos.checkers() ? generate<EVASIONS>(pos, moveList) : generate<NON_EVASIONS>(pos, moveList);
    while (cur != moveList)
        if (((pinned & cur->from_sq()) || cur->from_sq() == ksq || cur->type_of() == EN_PASSANT)
            && !pos.legal(*cur)) // in special cases extra check legality
            *cur = *(--moveList); // swap with last
        else                      // memory is not cleared
            ++cur;                // who cares?

    return moveList;
}
    
