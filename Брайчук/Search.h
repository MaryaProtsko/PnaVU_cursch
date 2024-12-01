//
// Created by aleks on 24.11.2024.
//

#ifndef SKEET_ENGINE_SEARCH_H
#define SKEET_ENGINE_SEARCH_H

#include "MoveBoard.h"
#include "Eval.h"
#include "movegen.h"
#include <chrono>
#include <iostream>

enum ABType {
    PV,
    NON_PV
}; // unused for now


struct SearchInfo {
    Move bestMove, currMove;
    Value score, currScore;
    Depth depth, qDepth,selDepth;
    time_t duration;
    uint64_t nodes, aBNodes, qNodes;
};

using TimePoint = std::chrono::high_resolution_clock::time_point;

#define NOW std::chrono::high_resolution_clock::now()
#define MS_DUR(dur) std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()

struct SearchState {
    bool stopped, infinite;
    time_t duration;
    TimePoint startTime;
    int depth;
};

class Search {
public:

    Search(MoveBoard& bd):b(bd),eval(bd){};


    // calling starts search with options
    // determined by ss
    void think();



    const SearchInfo& info() const;
    SearchState& state();
    void set_state(SearchState state);

private:
    template<bool ROOT>
    Value alpha_beta(Depth d, Value alpha, Value beta, Depth ply);
    template<bool ROOT>
    Value quiesce(Depth d, Value alpha, Value beta);
    Value nega_max(Depth d); // unused

    Value iterative_deepening(Depth maxDepth);

    int rate_move(Move m);
    bool search_stopped();
    SearchInfo si;
    SearchState ss;
    Eval eval;
    MoveBoard& b;
};

inline const SearchInfo& Search::info() const { return si;}

inline SearchState& Search::state() { return ss;}

inline void Search::set_state(SearchState state) {ss = state;}

inline bool Search::search_stopped() {
    if (ss.stopped) return true;

    ss.stopped = ! (ss.infinite ||
            (ss.duration && MS_DUR(NOW - ss.startTime) < (ss.duration - 5))
            ||(ss.depth && (si.depth <= ss.depth)));
    //std::cout << ss.stopped << " " << ss.infinite << "\n";
    return ss.stopped;
}




#endif //SKEET_ENGINE_SEARCH_H
