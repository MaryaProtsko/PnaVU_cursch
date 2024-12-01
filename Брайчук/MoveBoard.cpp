#include <iostream>
#include "MoveBoard.h"
#include "movegen.h"
#include "ZorbistFenBoard.h"

// Tests whether a pseudo-legal move is legal
// used in movegen
// also can be applied to any move from other GenType
// generation
bool MoveBoard::legal(Move m) const {

    assert(m.is_ok());

    Color us = sideToMove;
    Square from = m.from_sq();
    Square to = m.to_sq();

    assert(color_of(moved_piece(m)) == us);
    assert(piece_on(square<KING>(us)) == make_piece(us, KING));

    // En passant captures are a tricky special case. Because they are rather
    // uncommon, do it simply by testing whether the king is attacked after
    // the move is made.
    if (m.type_of() == EN_PASSANT) {
        Square ksq = square<KING>(us);
        Square capsq = to - pawn_push(us);
        Bitboard occupied = (pieces() ^ from ^ capsq) | to;

        assert(to == ep_square());
        assert(moved_piece(m) == make_piece(us, PAWN));
        assert(piece_on(capsq) == make_piece(~us, PAWN));
        assert(piece_on(to) == NO_PIECE);

        return !(attacks_bb<ROOK>(ksq, occupied) & pieces(~us, QUEEN, ROOK))
               && !(attacks_bb<BISHOP>(ksq, occupied) & pieces(~us, QUEEN, BISHOP));
    }

    // Castling moves generation does not check if the castling path is clear of
    // enemy attacks, it is delayed at a later time: now!
    if (m.type_of() == CASTLING) {
        // real king to
        to = relative_square(us, to > from ? SQ_G1 : SQ_C1);
        Direction step = to > from ? WEST : EAST;

        for (Square s = to; s != from; s += step)
            if (attackers_to(s) & pieces(~us))
                return false;
    }

    // If the moving piece is a king, check whether the destination square is
    // attacked by the opponent.
    if (type_of(piece_on(from)) == KING)
        return !(attackers_to(to, pieces() ^ from) & pieces(~us));

    // A non-king move is legal if and only if it is not pinned or it
    // is moving along the ray towards or away from the king.
    return !(blockers_for_king(us) & from) || aligned(from, to, square<KING>(us));
}


// Takes a random move and tests whether the move is
// pseudo-legal. hard and slow.
bool MoveBoard::pseudo_legal(const Move m) const {

    Color us = sideToMove;
    Square from = m.from_sq();
    Square to = m.to_sq();
    Piece pc = moved_piece(m);

    // Use a slower but simpler function for uncommon cases
    // yet we skip the legality check of MoveList<LEGAL>().
    if (m.type_of() != NORMAL)
        return checkers() ? MoveList<EVASIONS>(*this).contains(m)
                          : MoveList<NON_EVASIONS>(*this).contains(m);

    // Is not a promotion, so the promotion piece must be empty
    assert(m.promotion_type() - KNIGHT == NO_PIECE_TYPE);

    // If the 'from' square is not occupied by a piece belonging to the side to
    // move, the move is obviously not legal.
    if (pc == NO_PIECE || color_of(pc) != us)
        return false;

    // The destination square cannot be occupied by a friendly piece
    if (pieces(us) & to)
        return false;

    // Handle the special case of a pawn move
    if (type_of(pc) == PAWN) {
        // We have already handled promotion moves, so destination cannot be on the 8th/1st rank
        if ((Rank8BB | Rank1BB) & to)
            return false;

        if (!(pawn_attacks_bb(us, from) & pieces(~us) & to)  // Not a capture
            && !((from + pawn_push(us) == to) && empty(to))  // Not a single push
            && !((from + 2 * pawn_push(us) == to)            // Not a double push
                 && (relative_rank(us, from) == RANK_2) && empty(to) && empty(to - pawn_push(us))))
            return false;
    } else if (!(attacks_bb(type_of(pc), from, pieces()) & to))
        return false;

    // Evasions generator already takes care to avoid some kind of illegal moves
    // and legal() relies on this. We therefore have to take care that the same
    // kind of moves are filtered out here.
    if (checkers()) {
        if (type_of(pc) != KING) {
            // Double check? In this case, a king move is required
            if (more_than_one(checkers()))
                return false;

            // Our move must be a blocking interposition or a capture of the checking piece
            if (!(between_bb(square<KING>(us), lsb(checkers())) & to))
                return false;
        }
            // In case of king moves under check we have to remove the king so as to catch
            // invalid moves like b1a1 when opposite queen is on c1.
        else if (attackers_to(to, pieces() ^ from) & pieces(~us))
            return false;
    }

    return true;
}


// Tests whether a pseudo-legal move gives a check
bool MoveBoard::gives_check(Move m) const {

    assert(m.is_ok());
    assert(color_of(moved_piece(m)) == sideToMove);

    Square from = m.from_sq();
    Square to = m.to_sq();

    // Is there a direct check?
    if (check_squares(type_of(piece_on(from))) & to)
        return true;

    // Is there a discovered check?
    if (blockers_for_king(~sideToMove) & from)
        return !aligned(from, to, square<KING>(~sideToMove))
        || m.type_of() == CASTLING; // in case of castling from will be king sq,
        // when castling he has to stay at the back rank
        // if it is blocker piece it means that there is an enemy
        // king on the rank cuz it is the only direction it can block

    switch (m.type_of()) {
        case NORMAL :
            return false;

        case PROMOTION :
            return attacks_bb(m.promotion_type(), to, pieces() ^ from) & square<KING>(~sideToMove);

            // En passant capture with check? We have already handled the case of direct
            // checks and ordinary discovered check, so the only case need to handle
            // is the unusual case of a discovered check through the captured pawn.
        case EN_PASSANT : {
            Square capsq = make_square(file_of(to), rank_of(from));
            Bitboard b = (pieces() ^ from ^ capsq) | to;

            return (attacks_bb<ROOK>(square<KING>(~sideToMove), b) & pieces(sideToMove, QUEEN, ROOK))
                   | (attacks_bb<BISHOP>(square<KING>(~sideToMove), b)
                      & pieces(sideToMove, QUEEN, BISHOP));
        }
        default :  //CASTLING
        {
            // Castling is encoded as 'king captures the rook'
            Square rto = relative_square(sideToMove, to > from ? SQ_F1 : SQ_D1);

            return check_squares(ROOK) & rto;
        }
    }
}


// Makes a move, and saves all information necessary
// to a DirtyState object. The move is assumed to be legal. Pseudo-legal
// moves should be filtered out before this function is called.
void MoveBoard::do_move(Move m, bool givesCheck) {

    assert(m.is_ok());

    // remember last state
    previousStates.push_back(curSt);

    Key k = curSt.key ^ Zobrist::side;

    // Increment ply counters. In particular, rule50 will be reset to zero later on
    // in case of a capture or a pawn move.
    ++gamePly;
    ++curSt.rule50;
    ++curSt.pliesFromNull;

    Color us = sideToMove;
    Color them = ~us;
    Square from = m.from_sq();
    Square to = m.to_sq();
    Piece pc = piece_on(from);
    Piece captured = m.type_of() == EN_PASSANT ? make_piece(them, PAWN) : piece_on(to);

    assert(color_of(pc) == us);
    assert(captured == NO_PIECE || color_of(captured) == (m.type_of() != CASTLING ? them : us));
    assert(type_of(captured) != KING);

    if (m.type_of() == CASTLING) {
        assert(pc == make_piece(us, KING));
        assert(captured == make_piece(us, ROOK));

        Square rfrom, rto;
        do_castling<true>(us, from, to, rfrom, rto);

        curSt.positionalValue[us] += positional_value_of(captured, rto) - positional_value_of(captured, rfrom);
        k ^= Zobrist::psq[captured][rfrom] ^ Zobrist::psq[captured][rto];
        captured = NO_PIECE;
    }

    if (captured) {
        Square capsq = to;


        if (type_of(captured) == PAWN) {
            if (m.type_of() == EN_PASSANT) {
                capsq -= pawn_push(us);

                assert(pc == make_piece(us, PAWN));
                assert(to == curSt.epSquare);
                assert(relative_rank(us, to) == RANK_6);
                assert(piece_on(to) == NO_PIECE);
                assert(piece_on(capsq) == make_piece(them, PAWN));
            }
        }

        // update eval info
        curSt.materialValue[them] -= PieceValue[captured];
        curSt.positionalValue[them] -= positional_value_of(captured, capsq);

        // Update board and piece lists
        remove_piece(capsq);

        // key
        k ^= Zobrist::psq[captured][capsq];

        // Reset rule 50 counter
        curSt.rule50 = 0;
    }

    // Update hash key
    k ^= Zobrist::psq[pc][from] ^ Zobrist::psq[pc][to];

    if (type_of(pc) != KING)
        curSt.positionalValue[us] += positional_value_of(pc, to) - positional_value_of(pc, from);
    else {
        Value newKingValue = positional_value_of(pc, to);
        curSt.positionalValue[us] += newKingValue - curSt.kingLastValue[us];
        curSt.kingLastValue[us] = newKingValue; // TODO has to be outside cuz
        //TODO the king pos value changes when a non-pawn piece is captured
    }

    // Reset en passant square
    if (curSt.epSquare != SQ_NONE) {
        k ^= Zobrist::enpassant[file_of(curSt.epSquare)];
        curSt.epSquare = SQ_NONE;
    }

    // Update castling rights if needed
    if (curSt.castlingRights && (castlingRightsMask[from] | castlingRightsMask[to])) {
        k ^= Zobrist::castling[curSt.castlingRights];
        curSt.castlingRights &= ~(castlingRightsMask[from] | castlingRightsMask[to]);
        k ^= Zobrist::castling[curSt.castlingRights];
    }

    // Move the piece. The castling is handled earlier
    if (m.type_of() != CASTLING) {
        move_piece(from, to);
    }

    // If the moving piece is a pawn do some special extra work
    if (type_of(pc) == PAWN) {
        // Set en passant square if the moved pawn can be captured
        if ((int(to) ^ int(from)) == 16 // same as abs = 16
            && (pawn_attacks_bb(us, to - pawn_push(us)) & pieces(them, PAWN))) {
            curSt.epSquare = to - pawn_push(us);
            k ^= Zobrist::enpassant[file_of(curSt.epSquare)];
        } else if (m.type_of() == PROMOTION) {
            Piece promotion = make_piece(us, m.promotion_type());
            PieceType promotionType = type_of(promotion);

            assert(relative_rank(us, to) == RANK_8);
            assert(type_of(promotion) >= KNIGHT && type_of(promotion) <= QUEEN);

            remove_piece(to);
            put_piece(promotion, to);


            // Update hash keys
            k ^= Zobrist::psq[pc][to] ^ Zobrist::psq[promotion][to];


            // Update material
            curSt.materialValue[us] += PieceValue[promotion] - PieceValue[pc];
            curSt.positionalValue[us] += positional_value_of(promotion, to) - positional_value_of(pc, to);
        }

        // Reset rule 50 draw counter
        curSt.rule50 = 0;
    }

    // Set captured piece
    curSt.capturedPiece = captured;

    // Update the key with the final value
    curSt.key = k;

    // Calculate checkers bitboard (if move gives check)
    curSt.checkersBB = givesCheck ? attackers_to(square<KING>(them)) & pieces(us) : 0;

    sideToMove = ~sideToMove;

    // Update king attacks used for fast check detection
    set_check_info();

    // Calculate the repetition info. It is the ply distance from the previous
    // occurrence of the same position, negative in the 3-fold case, or zero
    // if the position was not repeated.
    curSt.repetition = 0;

    //std::cout << curSt.key << "\n\n";

    int end = std::min(curSt.rule50, curSt.pliesFromNull);
    if (end >= 4) {

        auto stp = previousStates.end() - 2;
        for (int i = 4; i <= end; i += 2) {
            stp -= 2;
            if (stp->key == curSt.key) {
                //std::cout << "111\n";
                curSt.repetition = stp->repetition ? -i : i;
                break;
            }
            //2 * 5 * 3 + 1 *
            //std::cout << stp-> key << "\n";

        }
    }
    //std::cout << "\n\n\n";
    //std::cout << key() << "\n";
    assert(pos_is_ok());
}


// Unmakes a move. When it returns, the position should
// be restored to exactly the same state as before the move was made.
void MoveBoard::undo_move(Move m) {

    assert(m.is_ok());


    // go back to previous state
    DirtyState st = curSt;
    curSt = previousStates.back();
    previousStates.pop_back();

    sideToMove = ~sideToMove;

    Color us = sideToMove;
    Square from = m.from_sq();
    Square to = m.to_sq();
    Piece pc = piece_on(to);

    assert(empty(from) || m.type_of() == CASTLING);
    assert(type_of(st.capturedPiece) != KING);

    if (m.type_of() == PROMOTION) {
        assert(relative_rank(us, to) == RANK_8);
        assert(type_of(pc) == m.promotion_type());
        assert(type_of(pc) >= KNIGHT && type_of(pc) <= QUEEN);

        remove_piece(to);
        pc = make_piece(us, PAWN);
        put_piece(pc, to);
    }

    if (m.type_of() == CASTLING) {
        Square rfrom, rto;
        do_castling<false>(us, from, to, rfrom, rto);
    } else {
        move_piece(to, from);  // Put the piece back at the source square

        if (st.capturedPiece) {
            Square capsq = to;

            if (m.type_of() == EN_PASSANT) {
                capsq -= pawn_push(us);

                assert(type_of(pc) == PAWN);
                assert(to == curSt.epSquare);
                assert(relative_rank(us, to) == RANK_6);
                assert(piece_on(capsq) == NO_PIECE);
                assert(st.capturedPiece == make_piece(~us, PAWN));
            }

            put_piece(st.capturedPiece, capsq);  // Restore the captured piece
        }
    }

    --gamePly;

    assert(pos_is_ok());
}


// Helper used to do/undo a castling move.
template<bool Do>
void MoveBoard::do_castling(Color us, Square from, Square &to, Square &rfrom, Square &rto) {

    bool kingSide = to > from;
    rfrom = to;  // Castling is encoded as "king captures friendly rook"
    rto = relative_square(us, kingSide ? SQ_F1 : SQ_D1);
    to = relative_square(us, kingSide ? SQ_G1 : SQ_C1);

    remove_piece(Do ? from : to);
    remove_piece(Do ? rfrom : rto);
    board[Do ? from : to] = board[Do ? rfrom : rto] =
            NO_PIECE;  // remove_piece does not do this for us
    put_piece(make_piece(us, KING), Do ? to : from);
    put_piece(make_piece(us, ROOK), Do ? rto : rfrom);
}


// Used to do a "null move": it flips
// the side to move without executing any move on the board.
void MoveBoard::do_null_move() {

    assert(!checkers());

    previousStates.push_back(curSt);


    if (curSt.epSquare != SQ_NONE) {
        curSt.key ^= Zobrist::enpassant[file_of(curSt.epSquare)];
        curSt.epSquare = SQ_NONE;
    }

    curSt.key ^= Zobrist::side;
    ++curSt.rule50;

    curSt.pliesFromNull = 0;

    sideToMove = ~sideToMove;

    set_check_info();

    curSt.repetition = 0;

    assert(pos_is_ok());
}


// Must be used to undo a "null move"
void MoveBoard::undo_null_move() {

    assert(!checkers());
    curSt = previousStates.back();
    previousStates.pop_back();
    sideToMove = ~sideToMove;
}


// Computes the new hash key after the given move.
// It doesn't recognize special moves like castling,
// en passant and promotions.
Key MoveBoard::key_after(Move m) const {

    Square from = m.from_sq();
    Square to = m.to_sq();
    Piece pc = piece_on(from);
    Piece captured = piece_on(to);
    Key k = curSt.key ^ Zobrist::side;

    if (captured)
        k ^= Zobrist::psq[captured][to];

    k ^= Zobrist::psq[pc][to] ^ Zobrist::psq[pc][from];

    return (captured || type_of(pc) == PAWN) ? k : k;//adjust_key50<true>(k);
}


// Tests if the SEE (Static Exchange Evaluation)
// value of move is greater or equal to the given threshold.
// algorithm similar to alpha-beta pruning with a null window.
bool MoveBoard::see_ge(Move m, int threshold) const {

    assert(m.is_ok());

    // Only deal with normal moves, assume others pass a simple SEE
    if (m.type_of() != NORMAL)
        return VALUE_ZERO >= threshold;

    Square from = m.from_sq(), to = m.to_sq();

    int swap = PieceValue[piece_on(to)] - threshold;
    if (swap < 0)
        return false;

    swap = PieceValue[piece_on(from)] - swap;
    if (swap <= 0)
        return true;

    assert(color_of(piece_on(from)) == sideToMove);
    Bitboard occupied = pieces() ^ from ^ to;  // xoring to is important for pinned piece logic
    Color stm = sideToMove;
    Bitboard attackers = attackers_to(to, occupied);
    Bitboard stmAttackers, bb;
    int res = 1;

    while (true) {
        stm = ~stm;
        attackers &= occupied;

        // If stm has no more attackers then give up: stm loses
        if (!(stmAttackers = attackers & pieces(stm)))
            break;

        // Don't allow pinned pieces to attack as long as there are
        // pinners on their original square.
        if (pinners(~stm) & occupied) {
            stmAttackers &= ~blockers_for_king(stm);

            if (!stmAttackers)
                break;
        }

        res ^= 1;

        // Locate and remove the next least valuable attacker, and add to
        // the bitboard 'attackers' any X-ray attackers behind it.
        if ((bb = stmAttackers & pieces(PAWN))) {
            if ((swap = PawnValue - swap) < res)
                break;
            occupied ^= least_significant_square_bb(bb);

            attackers |= attacks_bb<BISHOP>(to, occupied) & pieces(BISHOP, QUEEN);
        } else if ((bb = stmAttackers & pieces(KNIGHT))) {
            if ((swap = KnightValue - swap) < res)
                break;
            occupied ^= least_significant_square_bb(bb);
        } else if ((bb = stmAttackers & pieces(BISHOP))) {
            if ((swap = BishopValue - swap) < res)
                break;
            occupied ^= least_significant_square_bb(bb);

            attackers |= attacks_bb<BISHOP>(to, occupied) & pieces(BISHOP, QUEEN);
        } else if ((bb = stmAttackers & pieces(ROOK))) {
            if ((swap = RookValue - swap) < res)
                break;
            occupied ^= least_significant_square_bb(bb);

            attackers |= attacks_bb<ROOK>(to, occupied) & pieces(ROOK, QUEEN);
        } else if ((bb = stmAttackers & pieces(QUEEN))) {
            if ((swap = QueenValue - swap) < res)
                break;
            occupied ^= least_significant_square_bb(bb);

            attackers |= (attacks_bb<BISHOP>(to, occupied) & pieces(BISHOP, QUEEN))
                         | (attacks_bb<ROOK>(to, occupied) & pieces(ROOK, QUEEN));
        } else  // KING
            // If we "capture" with the king but the opponent still has attackers,
            // reverse the result.
            return (attackers & ~pieces(stm)) ? res ^ 1 : res;
    }

    return bool(res);
}
