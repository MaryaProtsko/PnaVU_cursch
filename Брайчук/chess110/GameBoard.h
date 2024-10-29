//
// Created by cs on 25.10.2024.
//

#ifndef CHESS110_GAMEBOARD_H
#define CHESS110_GAMEBOARD_H

#include "SimpleBoard.h"
#include "Move.h"
#include "types.h"
#include <list>
#include <string>


namespace Chess {

    struct GameBoardState {

        // Copied when making a move
        int castlingRights;
        int rule50;
        Square epSquare;

        // Not copied when making a move (will be recomputed anyhow)
        Piece capturedPiece;
        int repetition;

    };



    class GameBoard : public SimpleBoard {

    public:

        //GameBoard (const GameBoard &)            = delete;
        //GameBoard & operator=(const GameBoard &) = delete;

        // these consts used for move generation, prob have to be in different obj

        constexpr static Offset PIECE_OFFSETS[16] = {
                // rook
                 NORTH, EAST, SOUTH, WEST,
                // bishop
                NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST,
                // knight
                 NORTH_EAST + NORTH,NORTH_EAST + EAST, SOUTH_EAST + EAST, SOUTH_EAST + SOUTH,
                SOUTH_WEST + SOUTH, SOUTH_WEST + WEST, NORTH_WEST + WEST, NORTH_WEST + NORTH
        };

        constexpr static Square castingRookSquare[BLACK_OOO + 1] = {
                SQ_NONE,SQ_H1, SQ_A1, SQ_NONE,SQ_H8, SQ_NONE,SQ_NONE,SQ_NONE,SQ_A8
        };





        static int squaresToEdge[SQUARE_NB][16];
        static bool squaresToEdgeInited;
        static void initSquaresToEdge();
        static int castlingRightsMask[SQUARE_NB];



        GameBoard();

        void set(const std::string& fenStr);
        std::string fen() const;

        void generateSlidingMoves(std::vector<Move>& moves, Square from, Color us,int fromOffset, int toOffset);
        void generateHoppingMoves(std::vector<Move>& moves, Square from, Color us,int fromOffset, int toOffset);
        void generatePawnMoves(std::vector<Move>& moves, Square from, Color us);
        static void generatePromotions(std::vector<Move> &moves, Square from, Square target, Color us);
        void generateCastlingMoves(std::vector<Move>& moves, Color us);
        std::vector<Move> generatePseudoLegalMoves(Chess::Color us);

        std::vector<Move> generateLegalMoves();

        bool isInCheck(Color us, Square sq = SQ_NONE);

        void doCastling(Color us, Square from, Square& to, Square& rfrom, Square& rto, bool doUndo);


        void doMove(Move move, DirtyPiece* dp = nullptr);
        void undoMove(Move move);

        static Direction pawnDirection(Color c);

        bool canCastle(CastlingRights cr) const { return (stateStack.back().castlingRights) & cr; }

        CastlingRights castlingRightsFor(Color c) const {
            return c & CastlingRights(stateStack.back().castlingRights);
        }

    private:
        Color sideToMove;
        int plyCount;
        Square kingPos[COLOR_NB];
        std::list<GameBoardState> stateStack;
    };



}




#endif //CHESS110_GAMEBOARD_H
