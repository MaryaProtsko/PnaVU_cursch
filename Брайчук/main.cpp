#include <iostream>
#include <cstdint>
#include "test2.h"
#include "MoveBoard.h"
#include "movegen.h"
#include "types.h"
#include "bitboard.h"
#include "Search.h"
#include "UCI.h"

int perftRoot(MoveBoard& b, int depth){
    if (!depth) return 1;//b.is_draw(30) != 0;
    int count = 0;
    //std::cout << "\n";
    for (const auto& m: MoveList<LEGAL>(b)){

        //std::cout << toStr(Move(m)) << "\n";
        //std::cout << b.dirtyStateStack.back().castlingRights << "\n";
        b.do_move(Move(m));
        count += perftRoot(b, depth-1);
        //count += b.is_draw(30);
        b.undo_move(Move(m));
    }
    return count;
}

void perft(){

    Bitboards::init();
    ZorbistFenBoard::init();
    MoveBoard b;
    //b.set("5r1k/4Qpq1/4p3/1p1p2P1/2p2P2/1p2P3/3P4/BK6 b - - 0 1"); //TODO test qs
    //b.set("5r1k/4Qpq1/4p3/1p1p2P1/2p2P2/4P3/1p1P4/BK6 w - - 0 2");
    //b.set("5r1k/4Q1q1/4pB2/1p1p2P1/2p2P2/1p2P3/3P4/1K6 b - - 0 2");
    //b.set("2q1nk1r/4Rp2/1ppp1P2/6Pp/3p1B2/3P3P/PPP1Q3/6K1 w");
   //b.set("8/8/8/8/8/8/2r5/KNrk4 w - - 0 1");

    //b.set("rnbqkb1r/pppppppp/5n2/8/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 0 1");
    //b.do_move(Move(SQ_F6, SQ_E4));
    //Eval eval(b);



    /*b.set("rnbq1k1r/pp1Pbppp/2p5/6P1/2B5/8/PPP1Nn1P/RNBQK2R b KQ - 1 8");

    Eval eval(b);
    std::cout << b << "\n";
    std::cout << eval.eval();
    b.do_move(Move(SQ_D8, SQ_E8));*/



    //perftRoot(b,1);
    //std::cout << Eval(b).eval() << "\n";
    //Search s(b);

    //s.think();


    b.set("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    std::cout << b.fen() << "\n";
    /*b.do_move(Move(SQ_A1, SQ_B1));
    b.do_move(Move(SQ_H8, SQ_H7));
    b.do_move(Move(SQ_B1, SQ_A1));
    b.do_move(Move(SQ_H7, SQ_H8));*/
    for (int d = 1; d <= 6; d++)
      std::cout << d << " "<< perftRoot(b, d) << "\n";
    //b.set("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

}

int main() {
    //std::cout << "Hello, World!" << std::endl;

    //perft();
    //getchar();
    UCI uci;
    uci.loop();

    return 0;
}
