//
// Created by cs on 25.10.2024.
//


#include "GameBoard.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include <memory>
#include <iosfwd>
#include <ios>
#include <algorithm>
#include <cstring>


namespace Chess {

    namespace {

        constexpr std::string_view PieceToChar(" PNBRQK  pnbrqk");

        Piece Pieces[] = {Piece(WHITE, PAWN), Piece(WHITE, KNIGHT), Piece(WHITE, BISHOP), Piece(WHITE, ROOK), Piece(WHITE, QUEEN), Piece(WHITE, KING),
                          Piece(~WHITE, PAWN), Piece(~WHITE, KNIGHT), Piece(~WHITE, BISHOP), Piece(~WHITE, ROOK), Piece(~WHITE, QUEEN), Piece(~WHITE, KING)};
    }  // namespace

    GameBoard::GameBoard() {
        initSquaresToEdge();
    }

    bool GameBoard::squaresToEdgeInited = false;
    int GameBoard::squaresToEdge[SQUARE_NB][16] = {};
    int GameBoard::castlingRightsMask[] = {};

    void GameBoard::initSquaresToEdge() {
        if (squaresToEdgeInited) return;
        for (int sq = 0; sq < SQUARE_NB; sq++){
            int rank = rankOf((Square)sq);
            int file = fileOf((Square)sq);
            int north = 7 - rank;
            int east = 7 - file;
            int south = rank;
            int west = file;
            squaresToEdge[sq][0] = north;
            squaresToEdge[sq][1] = east;
            squaresToEdge[sq][2] = south;
            squaresToEdge[sq][3] = west;
            squaresToEdge[sq][4] = std::min(north, east);
            squaresToEdge[sq][5] = std::min(south, east);
            squaresToEdge[sq][6] = std::min(south, west);
            squaresToEdge[sq][7] = std::min(north, west);
            squaresToEdge[sq][8] = rank <= RANK_6 && file <= FILE_G ? 1 : 0;
            squaresToEdge[sq][9] = rank <= RANK_7 && file <= FILE_F ? 1 : 0;
            squaresToEdge[sq][10] = rank >= RANK_2 && file <= FILE_F ? 1 : 0;
            squaresToEdge[sq][11] = rank >= RANK_3 && file <= FILE_G ? 1 : 0;
            squaresToEdge[sq][12] = rank >= RANK_3 && file >= FILE_B ? 1 : 0;
            squaresToEdge[sq][13] = rank >= RANK_2 && file >= FILE_C ? 1 : 0;
            squaresToEdge[sq][14] = rank <= RANK_7 && file >= FILE_C ? 1 : 0;
            squaresToEdge[sq][15] = rank <= RANK_6 && file >= FILE_B ? 1 : 0;
        }

        for (Color c : {WHITE , BLACK}) {
            castlingRightsMask[relativeSquare(c, SQ_A1)] = c & QUEEN_SIDE;
            castlingRightsMask[relativeSquare(c, SQ_E1)] = c & ANY_CASTLING;
            castlingRightsMask[relativeSquare(c, SQ_E1)] = c & KING_SIDE;
        }


        squaresToEdgeInited = true;
    }

    void GameBoard::generateSlidingMoves(std::vector<Move> &moves, Square from, Color us, int fromOffset, int toOffset) {
        Color them = ~(pieceAt(from).color());
        for (int i = fromOffset; i < toOffset; i++){
            Offset offset = PIECE_OFFSETS[i];
            Square target = from;
            for (int n = 0; n < squaresToEdge[from][i]; n++){
                target += offset;
                Piece blocker = pieceAt(target);

                if (!bool(blocker)){
                    moves.push_back(Move(from, target));
                    continue;
                }

                if (blocker.color() == them)
                    moves.push_back(Move(from, target));
                break;
            }
        }
    }
    void GameBoard::generateHoppingMoves(std::vector<Move> &moves, Square from,Color us, int fromOffset, int toOffset) {
        Color them = ~us;
        for (int i = fromOffset; i < toOffset; i++) {
            if (!squaresToEdge[from][i]) continue;
            Square target = from + PIECE_OFFSETS[i];
            Piece blocker = pieceAt(target);

            if (!bool(blocker) || blocker.color() == them)
                moves.push_back(Move(from, target));

        }
    }

    Direction GameBoard::pawnDirection(Chess::Color c) { return c == WHITE ? NORTH : SOUTH; }

    void GameBoard::generatePromotions(std::vector<Move> &moves, Square from, Square target, Color us){
        if (relativeRank(us, target) != RANK_8){
            moves.push_back(Move(from, target));
            return;
        }

        for (int i = KNIGHT; i <= QUEEN; i++)
            moves.push_back(Move(from, target, PROMOTION, PieceType(i)));
    }

    void GameBoard::generatePawnMoves(std::vector<Move> &moves, Square from, Color us) {
        Color them = ~us;
        Offset offset = pawnDirection(us);
        for (Offset captureDir : {EAST , WEST}){
            if (fileOf(from) == FILE_A && captureDir == WEST || fileOf(from) == FILE_H && captureDir == EAST) continue;
            Offset captureOffset = offset + captureDir;
            Square target = from + captureOffset;
            if (target == stateStack.back().epSquare) {
                moves.push_back(Move(from, target, EN_PASSANT));
                continue;
            }
            Piece toCapture = pieceAt(target);
            if (bool(toCapture) && toCapture.color() == them)
                generatePromotions(moves, from, target, us);


        }
        Square target = from + offset;
        if (!isEmpty(target)) return;
        generatePromotions(moves, from, target, us);
        if (relativeRank(us, from) != RANK_2) return;
        target+=offset;
        if (!isEmpty(target)) return;
        moves.push_back(Move(from, target));
    }

    void GameBoard::generateCastlingMoves(std::vector<Move> &moves, Chess::Color us) {
        for (CastlingRights right : {us & KING_SIDE, us & QUEEN_SIDE}){
            if (!canCastle(right)) continue;
            Square kingSq = kingPos[us];
            Square rookSq = right & KING_SIDE ? relativeSquare(us, SQ_H1) : relativeSquare(us, SQ_A1);
            Offset offset = kingSq < rookSq ? EAST : WEST;
            bool isBlocked = false;
            for (Square sq = kingSq + offset; sq != rookSq; sq+=offset){
                if (!isEmpty(sq)){
                    isBlocked = true;
                    break;
                }
            }
            if(!isBlocked)
            moves.push_back(Move(kingSq, rookSq, CASTLING));
        }
    }

    std::vector<Move> GameBoard::generatePseudoLegalMoves(Chess::Color us) {
        std::vector<Move> moves;
        auto pieces = piecesOfColor(us);
        for (const Square& sq : pieces){
            switch (pieceAt(sq).pieceType()) {
                case PAWN:
                    generatePawnMoves(moves, sq, us);
                    break;
                case KNIGHT:
                    generateHoppingMoves(moves, sq, us, 8, 16);
                    break;
                case BISHOP:
                    generateSlidingMoves(moves, sq, us, 4, 8);
                    break;
                case ROOK:
                    generateSlidingMoves(moves, sq, us, 0, 4);
                    break;
                case QUEEN:
                    generateSlidingMoves(moves, sq, us, 0, 8);
                    break;
                case KING:
                    generateHoppingMoves(moves, sq, us, 0, 8);
                    generateCastlingMoves(moves, us);
                    break;
            }
        }
        return moves;
    }

    bool GameBoard::isInCheck(Color us, Square sq){
        if(sq == SQ_NONE) sq = kingPos[us];
        auto response =  generatePseudoLegalMoves(~us);

        for (Move resp : response){
            if (resp.to() == sq)
                return true;

        }
        return false;
    }

    std::vector<Move> GameBoard::generateLegalMoves() {
        auto pseudo = generatePseudoLegalMoves( sideToMove);
        std::vector<Move> legalMoves;

        for (Move m : pseudo){
            doMove(m);

            if (m.moveType() == CASTLING){
                Offset offset = m.from() < m.to() ? EAST : WEST;
                bool passChecked = false;
                for (Square sq = m.from() + offset; sq != m.from() + 2 * offset; sq+=offset){
                    if (!isInCheck(~sideToMove)){
                        passChecked = true;
                        break;
                    }
                }
                if (passChecked) {
                    undoMove(m);
                    continue;
                }
            }

            if (!isInCheck(~sideToMove)) legalMoves.push_back(m);

            undoMove(m);
        }

        //std::cout << legalMoves.size() << std::endl;

        return legalMoves;
    }

    // Makes a move, and saves all information necessary
    // to a StateInfo object. The move is assumed to be legal. Pseudo-legal
    // moves should be filtered out before this function is called.
    void GameBoard::doMove(Move m, DirtyPiece* dp) { // TODO ADD ZORBIST
                                                    // TODO DIRTYPIECE
        //std::cout << "do " << char( m.from() % 8 + 'a') << char(m.from() / 8 + '1') << "-" << char( m.to() % 8 + 'a') << char(m.to() / 8 + '1')<< std::endl;
        assert(m.isMove());

        // Copy some fields of the old state to our new StateInfo object except the
        // ones which are going to be recalculated from scratch anyway and then switch
        // our state pointer to point to the new (ready to be updated) state.
        GameBoardState newState;
        //newState = stateStack.back();

        // Increment ply counters. In particular, rule50 will be reset to zero later on
        // in case of a capture or a pawn move.
        ++plyCount;
        newState.rule50 = stateStack.back().rule50+1;

        Color  us = sideToMove;
        Color  them = ~us;
        Square from = m.from();
        Square to = m.to();
        Piece  pc = pieceAt(from);
        Piece  captured = m.moveType() == EN_PASSANT ? Piece(them, PAWN) : pieceAt(to);

        //std::cout << "move " << char( m.from() % 8 + 'a') << char(m.from() / 8 + '1') << "-" << char( m.to() % 8 + 'a') << char(m.to() / 8 + '1')<< std::endl;
        //std::cout << fen() << "\n";
        assert(pc.color() == us);
        assert(!bool(captured) || captured.color() == (m.moveType() != CASTLING ? them : us));
        assert(captured.pieceType() != KING);

        if (m.moveType() == CASTLING)
        {
            assert(pc == Piece(us, KING));
            assert(captured == Piece(us, ROOK));

            Square rfrom, rto; // will be needed in zorbist
            doCastling(us, from, to, rfrom, rto, true);

            captured = Piece(NO_PIECE_TYPE);
        }

        if (bool(captured))
        {
            Square capsq = to;
            if (captured.pieceType() == PAWN)
            {
                if (m.moveType() == EN_PASSANT)
                {
                    capsq -= pawnDirection(us);

                    assert(pc == Piece(us, PAWN));
                    assert(to == stateStack.back().epSquare);
                    assert(relativeRank(us, to) == RANK_6);
                    assert(!bool(pieceAt(to)));
                    assert(pieceAt(capsq) == Piece(them, PAWN));
                }

            }

            // Update board and piece lists
            deletePiece(capsq);
            // Reset rule 50 counter
            newState.rule50 = 0;
        }

        // Reset en passant square
        if (newState.epSquare != SQ_NONE)
        {
            newState.epSquare = SQ_NONE;
        }

        newState.castlingRights = stateStack.back().castlingRights;
        // Update castling rights if needed
        if (newState.castlingRights && (castlingRightsMask[from] | castlingRightsMask[to]))
        {
            newState.castlingRights &= ~(castlingRightsMask[from] | castlingRightsMask[to]);
        }

        // Move the piece. Castling is handled earlier
        if (m.moveType() != CASTLING)
        {
            movePiece(from, to);
            if (pc.pieceType() == KING)
                kingPos[us] = to;
        }

        // If the moving piece is a pawn do some special extra work
        if (pc.pieceType() == PAWN)
        {
            // Set en passant square if the moved pawn can be captured
            if ((int(to) ^ int(from)) == 16) // TODO HAVE TO ADD EXTRA LOGIC FOR STANDARD
            {
                newState.epSquare = to - pawnDirection(us);
            }

            else if (m.moveType() == PROMOTION)
            {
                Piece promotion = Piece(us, m.promotionType());

                assert(relativeRank(us, to) == RANK_8);
                assert(promotion.pieceType() >= KNIGHT && promotion.pieceType() <= QUEEN);

                deletePiece(to);
                addPiece(to, promotion);

            }
            // Reset rule 50 draw counter
            newState.rule50 = 0;
        }

        // Set capture piece
        newState.capturedPiece = captured;

        sideToMove = ~sideToMove;
        // TODO CALCULATE REPETITION.

        stateStack.push_back(newState);

        //std::cout << *this;


        //assert(posIsOk()); //TODO
    }

    // Unmakes a move. When it returns, the position should
    // be restored to exactly the same state as before the move was made.
    void GameBoard::undoMove(Move m) {

        assert(m.isMove());

        sideToMove = ~sideToMove;

        Color  us   = sideToMove;
        Square from = m.from();
        Square to   = m.to();
        Piece  pc   = pieceAt(to);
        //std::cout << "undo move " << char( m.from() % 8 + 'a') << char(m.from() / 8 + '1') << "-" << char( m.to() % 8 + 'a') << char(m.to() / 8 + '1')<< std::endl;

        assert(isEmpty(from) || m.moveType() == CASTLING);
        assert(stateStack.back().capturedPiece.pieceType() != KING);

        if (m.moveType() == PROMOTION)
        {
            assert(relativeRank(us, to) == RANK_8);
            assert(pc.pieceType() == m.promotionType());
            assert(pc.pieceType()  >= KNIGHT && pc.pieceType()  <= QUEEN);

            deletePiece(to);
            addPiece(to, Piece(us, PAWN));
        }

        if (m.moveType() == CASTLING)
        {
            Square rfrom, rto;
            doCastling(us, from, to, rfrom, rto, false);
        }
        else
        {
            movePiece(to, from);  // Put the piece back at the source square
            if (pc.pieceType() == KING)
                kingPos[us] = from;

            if (bool(stateStack.back().capturedPiece))
            {
                Square capsq = to;

                if (m.moveType() == EN_PASSANT)
                {

                    //auto previ = stateStack.end();

                    //std::cout << "undo " << char( m.from() % 8 + 'a') << char(m.from() / 8 + '1') << "-" << char( m.to() % 8 + 'a') << char(m.to() / 8 + '1')<< std::endl;
                    //std::cout << (--previ)->epSquare <<"\n";
                    //std::cout << stateStack.back().epSquare <<"\n";
                    capsq -= pawnDirection(us);

                    //assert(pc.pieceType() == PAWN);
                    //auto prev = stateStack.end();
                    //assert(to == (--prev)->epSquare);
                    //assert(relativeRank(us, to) == RANK_6);
                    //assert(!bool(pieceAt(capsq)));
                    assert(stateStack.back().capturedPiece == Piece(~us, PAWN));
                }

                addPiece(capsq,stateStack.back().capturedPiece);  // Restore the captured piece
            }
        }

        // Finally point our state pointer back to the previous state
        stateStack.pop_back();
        --plyCount;

        if (m.moveType() == EN_PASSANT)
        {
            Square capsq = to;
            capsq -= pawnDirection(us);

            assert(pc.pieceType() == PAWN);
            assert(to == stateStack.back().epSquare);
            assert(relativeRank(us, to) == RANK_6);
        }

        //assert(posIsOk());
    }


// Helper used to do/undo a castling move.

    void GameBoard::doCastling(Color us, Square from, Square& to, Square& rfrom, Square& rto, bool doUndo) {

        bool kingSide = to > from;
        rfrom = to;  // Castling is encoded as "king captures friendly rook"
        rto = relativeSquare(us, kingSide ? SQ_F1 : SQ_D1);
        to = relativeSquare(us, kingSide ? SQ_G1 : SQ_C1);


        movePiece(doUndo ? from : to, doUndo ? to : from);
        kingPos[us] = doUndo ? to : from;
        movePiece(doUndo ? rfrom : rto, doUndo ? rto : rfrom);

        //if (doUndo) std::cout << "111";

        // Remove both pieces first since squares could overlap in Chess960
        //deletePiece(doUndo ? from : to);
        //deletePiece(doUndo ? rfrom : rto);
        //board[doUndo ? from : to] = board[doUndo ? rfrom : rto] =
        //        NO_PIECE;  // deletePiece does not do this for us
        //addPiece(doUndo ? to : from,Piece(us, KING) );
        //addPiece(doUndo ? rto : rfrom,Piece(us, ROOK));
    }

    // Initializes the position object with the given FEN string.
    // This function is not very robust - make sure that input FENs are correct,
    // this is assumed to be the responsibility of the GUI.
    void GameBoard::set(const std::string& fenStr) {
        /*
       A FEN string defines a particular position using only the ASCII character set.

       A FEN string contains six fields separated by a space. The fields are:

       1) Piece placement (from white's perspective). Each rank is described, starting
          with rank 8 and ending with rank 1. Within each rank, the contents of each
          square are described from file A through file H. Following the Standard
          Algebraic Notation (SAN), each piece is identified by a single letter taken
          from the standard English names. White pieces are designated using upper-case
          letters ("PNBRQK") whilst Black uses lowercase ("pnbrqk"). Blank squares are
          noted using digits 1 through 8 (the number of blank squares), and "/"
          separates ranks.

       2) Active color. "w" means white moves next, "b" means black.

       3) Castling availability. If neither side can castle, this is "-". Otherwise,
          this has one or more letters: "K" (White can castle kingside), "Q" (White
          can castle queenside), "k" (Black can castle kingside), and/or "q" (Black
          can castle queenside).

       4) En passant target square (in algebraic notation). If there's no en passant
          target square, this is "-". If a pawn has just made a 2-square move, this
          is the position "behind" the pawn. Following X-FEN standard, this is recorded
          TODO only  if there is a pawn in position to make an en passant capture, and if
          there really is a pawn that might have advanced two squares.

       5) Halfmove clock. This is the number of halfmoves since the last pawn advance
          or capture. This is used to determine if a draw can be claimed under the
          fifty-move rule.

       6) Fullmove number. The number of the full move. It starts at 1, and is
          incremented after Black's move.
    */

        GameBoardState si;

        //std::memset(this, 0, sizeof(GameBoard));
        //std::memset(&si, 0, sizeof(GameBoardState));

        unsigned char col, row, token;
        size_t idx;
        Square sq = SQ_A8;
        std::istringstream ss(fenStr);


        ss >> std::noskipws;

        // 1. Piece placement
        while ((ss >> token) && !isspace(token))
        {
            if (isdigit(token))
                sq += (token - '0') * EAST;  // Advance the given number of files

            else if (token == '/')
                sq += 2 * SOUTH;

            else if ((idx = PieceToChar.find(token)) != std::string::npos)
            {
                Piece pc(idx);
                if (pc.pieceType() == KING)
                    kingPos[pc.color()] = sq;
                addPiece(sq, pc);
                sq += EAST;
            }
        }

        // 2. Active color
        ss >> token;
        sideToMove = (token == 'w' ? WHITE : BLACK);
        ss >> token;

        // 3. Castling availability.
        while ((ss >> token) && !isspace(token))
        {
            Color c    = islower(token) ? BLACK : WHITE;
            token = char(toupper(token));
            if (token == 'K')
                si.castlingRights |= (c & (KING_SIDE));
            else if (token == 'Q')
                si.castlingRights |= (c & (QUEEN_SIDE));
        }

        // 4. En passant square.
        // Ignore if square is invalid or not on side to move relative rank 6.
        bool enpassant = false;

        if (((ss >> col) && (col >= 'a' && col <= 'h'))
            && ((ss >> row) && (row == (sideToMove == WHITE ? '6' : '3'))))
        {
            si.epSquare = makeSquare(File(col - 'a'), Rank(row - '1'));

            // En passant square will be considered only if //TODO
            // a) side to move have a pawn threatening epSquare
            // b) there is an enemy pawn in front of epSquare
            // c) there is no piece on epSquare or behind epSquare
            enpassant = true;
        }

        if (!enpassant)
            si.epSquare = SQ_NONE;

        // 5-6. Halfmove clock and fullmove number
        ss >> std::skipws >> si.rule50 >> plyCount;

        // Convert from fullmove starting from 1 to gamePly starting from 0,
        // handle also common incorrect FEN with fullmove = 0.
        plyCount = std::max(2 * (plyCount - 1), 0) + (sideToMove == BLACK);

        //assert(posIsOk());

        stateStack.push_back(si);
    }


    std::ostream& operator<<(std::ostream& os, const GameBoard& pos) { // draws

        os << "\n +---+---+---+---+---+---+---+---+\n";

        for (Rank r = RANK_8; r >= RANK_1; r = Rank(r-1)) {
            for (File f = FILE_A; f <= FILE_H; f = File(f+1))
                os << " | " << PieceToChar[pos.pieceAt(makeSquare(f, r)).raw()];

            os << " | " << (1 + r) << "\n +---+---+---+---+---+---+---+---+\n";
        }

        os << "   a   b   c   d   e   f   g   h\n";
        return os;
    }


    // Returns a FEN representation of the position.
    std::string GameBoard::fen() const {

        int                emptyCnt;
        std::ostringstream ss;

        for (Rank r = RANK_8; r >= RANK_1; r = Rank(r-1))
        {
            for (File f = FILE_A; f <= FILE_H; f = File(f+1))
            {
                for (emptyCnt = 0; f <= FILE_H && isEmpty(makeSquare(f, r)); f = File(f+1))
                    ++emptyCnt;

                if (emptyCnt)
                    ss << emptyCnt;

                if (f <= FILE_H)
                    ss << PieceToChar[pieceAt(makeSquare(f, r)).raw()];
            }

            if (r > RANK_1)
                ss << '/';
        }

        ss << (sideToMove == WHITE ? " w " : " b ");

        if (WHITE_OO & stateStack.back().castlingRights)
            ss << 'K';

        if (WHITE_OOO & stateStack.back().castlingRights)
            ss << 'Q';

        if (BLACK_OO & stateStack.back().castlingRights)
            ss << 'k';

        if (BLACK_OOO & stateStack.back().castlingRights)
            ss << 'q';

        if (!(ANY_CASTLING & stateStack.back().castlingRights))
            ss << '-';

        ss << (stateStack.back().epSquare == SQ_NONE ? " - " : " ");

        return ss.str();
    }


}