
#include <vector>
#include <iostream>
#include "Search.h"
#include <chrono>
#include "UCI.h"

void Search::think() {
    memset(&si, 0, sizeof(si));
    si.bestMove = si.currMove = Move::none();
    si.score = si.currScore =VALUE_NONE;
    iterative_deepening(ss.depth ? ss.depth : 128);
    ss.stopped = true;
}

Value Search::iterative_deepening(Depth maxDepth) {
    //Value val;
    //Move bestMoveCur = Move::none();
    for (Depth d = 1; d <= maxDepth; d++) { // results of previous depths help with move ordering
        si.currScore = alpha_beta<true>(d, -VALUE_INFINITE, VALUE_INFINITE, 0);

        si.depth = d;


        //???

        if (search_stopped()){
            return si.score;
        }


        si.bestMove = si.currMove;
        si.score = si.currScore;

        si.selDepth = si.depth + -si.qDepth - 1;
        si.nodes = si.aBNodes + si.qNodes;

        si.duration = MS_DUR(NOW - ss.startTime);
        //if (si.duration > ss.duration / 50 || ss.infinite || ss.depth)
        UCI::print_info(si);

        if (si.score > mate_in(MAX_PLY) || si.score < mated_in(MAX_PLY))
            break;



    }
    return si.score;
}

template<bool ROOT>
Value Search::alpha_beta(Depth d, Value alpha, Value beta, Depth ply) {
    si.aBNodes++;
    if (d > si.depth) si.depth = d; // counters


    if (b.is_draw(ply)) // does not detect stalemate
        return VALUE_DRAW;

    // if the depth reaches zero, dive into quiescence search
    if (!d)
    return quiesce<ROOT>(d - 1,alpha, beta);


    auto moveList = MoveList<LEGAL>(b);

    if (!moveList.size()) {
        return b.checkers() ?
        mated_in(0) // no moves in check means mate
        : VALUE_DRAW; // no moves without check means stalemate
    }

    std::vector<ExtMove> legalMoves;

    for (const auto &m: moveList) {
        legalMoves.push_back(m);
        // ? mb need to filter some
    }

    for (ExtMove &m: legalMoves) {
        m.value = rate_move(m); // giving a score to each move
        //filter? TODO
        //std::cout << m.value << "\n";
    }

    std::sort(legalMoves.begin(), legalMoves.end(), [](const ExtMove e1, const ExtMove e2) { return e1 > e2; });
    // move ordering


    Value bestVal = -VALUE_INFINITE; // using fail-soft aproach

    for (auto m: legalMoves) {
        // affects the search, so the info from the last
        // iteration CAN'T be used
        if (search_stopped()) return alpha;

        b.do_move(m);
        // negamax framework. see https://www.chessprogramming.org/Negamax
        int currVal = -alpha_beta<false>(d - 1, -beta, -alpha, ply + 1);
        b.undo_move(m);

        if (currVal > bestVal) {
            bestVal = currVal;
            if (currVal > alpha) {
                alpha = currVal;
                if (ROOT) si.currMove = m; // if the move exceeds alpha
            }
        }

        if (currVal >= beta) { // the move was too good
            return bestVal; // beta cut-off
        }
    }

    assert(bestVal != -VALUE_INFINITE);
    return bestVal;
}


// avoid (minimize) horizon effect
template<bool ROOT>
Value Search::quiesce(Depth d,Value alpha, Value beta) {
    si.qNodes++;
    if (d < si.qDepth) si.qDepth = d;

    int standPat = eval.eval();
    if (standPat >= beta)
        return beta;

    //the position's evaluation is used to establish a lower bound on the score
    if (alpha < standPat)
        alpha = standPat;



    auto moveList = MoveList<LEGAL>(b);

    std::vector<ExtMove> legalMoves;


    // filtering good captures
    for (const auto &m: moveList) {
        if (b.capture_stage(m)  && b.see_ge(m, -80))//|| b.checkers())
            //&& PieceValue[b.piece_on(m.to_sq())]-PieceValue[b.piece_on(m.from_sq())] > 0)
        legalMoves.push_back(m);
        // TODO search all evasions when in check?
    }
    //if (b.checkers() && legalMoves.empty())
    //    return mated_in(0);


    for (ExtMove &m: legalMoves) {
        m.value = rate_move(m);
        //std::cout << m.value << "\n";
    }

    std::sort(legalMoves.begin(), legalMoves.end(), [](const ExtMove e1, const ExtMove e2) { return e1 > e2; });

    //Value bestVal = standPat;

    // here using fail-hard approach, as anyway have to return standpat

    for (auto m: legalMoves) {
        if (search_stopped()) return alpha;
        b.do_move(m);
        int currVal = -quiesce<false>(d -1,-beta, -alpha);
        b.undo_move(m);

        if (currVal > beta)
            return beta;

        if (currVal > alpha){
            if (ROOT) si.currMove = m;
            alpha = currVal;
        }
    }

    return alpha;


}


Value Search::nega_max(Depth d) { // not used
    if (!d) return eval.eval();
    Value val = -VALUE_INFINITE;
    for (auto m: MoveList<LEGAL>(b)) {
        b.do_move(m);
        Value cur = -nega_max(d - 1);
        b.undo_move(m);
        val = std::max(val, cur);
    }
    return val;
}

int Search::rate_move(Move m) {
    int score = 0;

    // using the result of previous iteration
    if (m == si.bestMove)
        return 10000;

    // promotion is good
    if (m.type_of() == PROMOTION)
        score += PieceValue[m.promotion_type()] / 120;
    // TODO mb rate only QUEEN promotions to avoid searching all 4

    if (b.capture(m)) { // TODO using see_ge here costs much time
        Value capVal = PieceValue[b.piece_on(m.to_sq())] - PieceValue[b.piece_on(m.from_sq())]/10;
        score += capVal;
    }

    //if (b.see_ge(m, 1))
    //  score += 50;

    // checks are usually good and
    // after a check there are less moves - decreased search breadth
    if (b.gives_check(m))
        score += PawnValue;

    return score;
}
