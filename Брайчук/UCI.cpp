#include "UCI.h"
#include "bitboard.h"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

UCI::UCI() : search(b) {
}

void UCI::loop() {
    Bitboards::init();
    ZorbistFenBoard::init();

    string start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    std::string line;
    std::string token;

    SearchState& ss = search.state();
    ss.stopped = true;
    //cout.setf(ios::unitbuf);
    while (getline(cin, line)) {
        istringstream is(line);
        token.clear();
        is >> skipws >> token;

        if (token == "uci") {
            cout << "uciok" << endl;
        } else if (token == "isready") {
            cout << "readyok" << endl;
        } else if (token == "ucinewgame") {
            b.set(start);
        } else if (token == "position") {
            ss.stopped = true;
            is >> token;
            if (token == "startpos") {
                b.set(start);
            } else if (token == "fen") {
                std::string fen;
                while (is >> token && token != "moves") {
                    fen += token + " ";
                }
                fen.pop_back();
                b.set(fen);
            }

            while (is >> token) {
                if (token != "moves") { // make move
                    Move m = string_to_move(token);
                    b.do_move(m);
                }
            }
        } else if (token == "go") {
            if (ss.stopped) {
                while (is >> token) {
                    if (token == "movetime") {
                        is >> ss.duration;
                        ss.infinite = false;
                        ss.depth = 0;
                    } else if (token == "infinite"){
                        ss.infinite = true;
                        ss.depth = 0;
                        ss.duration = 0;
                    } else if (token == "depth"){
                        is >> ss.depth;
                        ss.infinite = false;
                        ss.duration = 0;
                    } else if (token == (b.side_to_move() == WHITE ? "wtime" : "btime")){
                        is >> ss.duration;
                        ss.duration /= 20;
                        ss.infinite = false;
                        ss.depth = 0;
                    }
                }
                ss.startTime = NOW;
            thread th1(&UCI::find_move, this);
            th1.detach();

            }

        } else if (token == "stop") {
            ss.stopped = true;
        } else if (token == "print") {
            cout << b;
            cout << endl;
        } else if (token == "quit") {
            break;
        }
    }
}


void UCI::find_move() {
    SearchState& ss = search.state();
    ss.stopped = false;
    search.think();
    Move bestMove = search.info().bestMove;


    std::cout << "bestmove " << toStr(bestMove) << endl;
    ss.stopped = true;
}

Move UCI::string_to_move(string &s) {

    auto moveList = MoveList<LEGAL>(b);

    for (Move m: moveList) {
        if (s == toStr(m)) {
            return m;
        }
    }
    cout << "INVALID" << endl;
    // TODO: Do we want to return empty move?
    return Move::null();

}

void UCI::print_info(SearchInfo info) {
    std::cout << "info depth " << info.depth
              << " seldepth " << info.selDepth
              << " nodes " << info.nodes
              << " score cp " << info.score
              << " pv " << toStr(info.bestMove) << std::endl;
}

