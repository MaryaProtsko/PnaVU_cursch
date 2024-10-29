#include <iostream>
#include <vector>
#include "Move.h"
#include "GameBoard.h"



int search(Chess::GameBoard& board, int depth){
    if (depth == 0) return 1;
    std::vector<Chess::Move> moves = board.generateLegalMoves();
    int count = 0;
    for (auto move : moves){
        board.doMove(move);
        count += search(board, depth - 1);
        board.undoMove(move);
        //std::cout << board.piecesOfColor(Chess::WHITE).size() +  board.piecesOfColor(Chess::BLACK).size() << "\n";
    }
    //std::cout << "done\n" ;
    return count;
}


int main() {
    std::cout << "Hello, World!" << std::endl;

    std::vector<Chess::Move> moves;

    Chess::GameBoard board;

    board.set("4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ");
    //board.set("7k/8/8/8/8/8/8/K7 w - - 0 1");

    //board.generateSlidingMoves(moves, Chess::SQ_A1,Chess::WHITE, 8, 16);



    for (int i = 1; i < 6; i++){ // for 6 does not work((( due to low speed
        std::cout << "perft depth " << i << ": " << search(board, i )<< " nodes" << std::endl;
    }


    return 0;
}
