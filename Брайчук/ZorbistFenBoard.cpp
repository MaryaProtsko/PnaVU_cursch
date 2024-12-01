#include "ZorbistFenBoard.h"


#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include <random>

#include "bitboard.h"


using std::string;

Square castlingRookSquare[CASTLING_RIGHT_NB];
Bitboard castlingPath[CASTLING_RIGHT_NB];
int castlingRightsMask[SQUARE_NB];

namespace Zobrist {

    Key psq[PIECE_NB][SQUARE_NB];
    Key enpassant[FILE_NB];
    Key castling[CASTLING_RIGHT_NB];
    Key side;
}

namespace {

    constexpr std::string_view PieceToChar(" PNBRQK  pnbrqk");

    constexpr Piece Pieces[] = {W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
                                B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING};
}  // namespace


// Returns an ASCII representation of the position
std::ostream &operator<<(std::ostream &os, const ZorbistFenBoard &pos) {
    os << "\n +---+---+---+---+---+---+---+---+\n";

    for (Rank r = RANK_8; r >= RANK_1; --r) {
        for (File f = FILE_A; f <= FILE_H; ++f)
            os << " | " << PieceToChar[pos.piece_on(make_square(f, r))];

        os << " | " << (1 + r) << "\n +---+---+---+---+---+---+---+---+\n";
    }

    os << "   a   b   c   d   e   f   g   h\n"
       << "\nFen: " << pos.fen() << "\nKey: " << std::hex << std::uppercase << std::setfill('0')
       << std::setw(16) << pos.key() << std::setfill(' ') << std::dec;
    return os;
}


// Initializes at startup the various arrays used to compute hash keys
void ZorbistFenBoard::init() {
    
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<unsigned long long> distribution;

    // also some castling initialization
    // the reason why the castling data is being stored is that
    // the castling places can be changed in chess960 which
    // is now not supported
    for (Color c : {WHITE, BLACK}){
        Square kingSq = relative_square(c, SQ_E1);
        Bitboard kSqBB = square_bb(kingSq);
        for (CastlingRights cr: {KING_SIDE, QUEEN_SIDE}){

            Square rookSq = relative_square(c, cr == KING_SIDE ? SQ_H1 : SQ_A1);
            Bitboard rSqBB = square_bb(rookSq);

            castlingRookSquare[c & cr] = rookSq;
            castlingPath[c & cr] = between_bb(kingSq, rookSq) & ~rSqBB;
            //std::cout << castlingPath[c & cr] << "\n";
            castlingRightsMask[rookSq] = c & cr;
        }
        castlingRightsMask[kingSq] = c & ANY_CASTLING;
    }

    for (Piece pc: Pieces)
        for (Square s = SQ_A1; s <= SQ_H8; ++s)
            Zobrist::psq[pc][s] = distribution(eng);

    for (File f = FILE_A; f <= FILE_H; ++f)
        Zobrist::enpassant[f] = distribution(eng);

    for (int cr = NO_CASTLING; cr <= ANY_CASTLING; ++cr)
        Zobrist::castling[cr] = distribution(eng);

    Zobrist::side = distribution(eng);
}


// Initializes the position object with the given FEN string.
// This function is not very reliable - make sure that input FENs are correct,
// this is assumed to be the responsibility of the GUI!!.
ZorbistFenBoard &ZorbistFenBoard::set(const string &fenStr) {
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
      only if there is a pawn in position to make an en passant capture, and if
      there really is a pawn that might have advanced two squares.

   5) Halfmove clock. This is the number of halfmoves since the last pawn advance
      or capture. This is used to determine if a draw can be claimed under the
      fifty-move rule.

   6) Fullmove number. The number of the full move. It starts at 1, and is
      incremented after Black's move.
*/

    unsigned char col, row, token;
    size_t idx;
    Square sq = SQ_A8;
    std::istringstream ss(fenStr);

    PieceBoard::reset();

    StateBoard::reset();


    ss >> std::noskipws;


    // 1. Piece placement
    while ((ss >> token) && !isspace(token)) {
        if (isdigit(token))
            sq += (token - '0') * EAST;  // Advance the given number of files

        else if (token == '/')
            sq += 2 * SOUTH; // after the end of n-th rank, we are on the (n+1)st rank first file
                             // need to go down 2 ranks

        else if ((idx = PieceToChar.find(token)) != string::npos) {
            put_piece(Piece(idx), sq);
            ++sq;
        }
    }

    // 2. Active color
    ss >> token;
    sideToMove = (token == 'w' ? WHITE : BLACK);
    ss >> token;

    // 3. Castling availability.

    while ((ss >> token) && !isspace(token)) {
        Color c = islower(token) ? BLACK : WHITE;

        token = char(toupper(token));

        if (token == 'K')
            curSt.castlingRights |= c & KING_SIDE; // overloaded &

        else if (token == 'Q')
            curSt.castlingRights |= c & QUEEN_SIDE;


    }

    // 4. En passant square.
    // Ignore if square is invalid or not on side to move relative rank 6.
    bool enpassant = false;

    if (((ss >> col) && (col >= 'a' && col <= 'h'))
        && ((ss >> row) && (row == (sideToMove == WHITE ? '6' : '3')))) {
        curSt.epSquare = make_square(File(col - 'a'), Rank(row - '1'));

        // En passant square will be considered only if
        // a) side to move have a pawn threatening epSquare
        // b) there is an enemy pawn in front of epSquare
        // c) there is no piece on epSquare or behind epSquare
        enpassant = pawn_attacks_bb(~sideToMove, curSt.epSquare) & pieces(sideToMove, PAWN)
                    && (pieces(~sideToMove, PAWN) & (curSt.epSquare + pawn_push(~sideToMove)))
                    && !(pieces() & (curSt.epSquare | (curSt.epSquare + pawn_push(sideToMove))));
    }

    if (!enpassant)
        curSt.epSquare = SQ_NONE;

    // 5-6. Halfmove clock and fullmove number
    ss >> std::skipws >> curSt.rule50 >> gamePly;

    // Convert from fullmove starting from 1 to gamePly starting from 0,
    // handle also common incorrect FEN with fullmove = 0.
    gamePly = std::max(2 * (gamePly - 1), 0) + (sideToMove == BLACK);


    set_state();

    assert(pos_is_ok());

    return *this;
}


// Sets king attacks to detect if a move gives check
void ZorbistFenBoard::set_check_info(){

    update_slider_blockers(WHITE);
    update_slider_blockers(BLACK);


    Square ksq = square<KING>(~sideToMove);

    curSt.checkSquares[PAWN] = pawn_attacks_bb(~sideToMove, ksq);
    curSt.checkSquares[KNIGHT] = attacks_bb<KNIGHT>(ksq);
    curSt.checkSquares[BISHOP] = attacks_bb<BISHOP>(ksq, pieces());
    curSt.checkSquares[ROOK] = attacks_bb<ROOK>(ksq, pieces());
    curSt.checkSquares[QUEEN] = curSt.checkSquares[BISHOP] | curSt.checkSquares[ROOK];
    curSt.checkSquares[KING] = 0;

}


// Computes the hash keys of the position, and other
// data that once computed is updated incrementally as moves are made.
// The function is only used when a new position is set up
void ZorbistFenBoard::set_state() {

    set_check_info();

    curSt.key = 0;
    curSt.materialValue[WHITE] = curSt.materialValue[BLACK] = VALUE_ZERO;
    curSt.positionalValue[WHITE] = curSt.positionalValue[BLACK] = VALUE_ZERO;
    curSt.kingLastValue[WHITE] = curSt.kingLastValue[BLACK] = VALUE_ZERO;
    curSt.checkersBB = attackers_to(square<KING>(sideToMove)) & pieces(~sideToMove);



    for (Bitboard b = pieces(); b;) {
        Square s = pop_lsb(b);
        Piece pc = piece_on(s);
        curSt.key ^= Zobrist::psq[pc][s];

        if (type_of(pc) != KING)
                curSt.materialValue[color_of(pc)] += PieceValue[pc];
    }


    for (Bitboard b = pieces(); b;) {
        Square s = pop_lsb(b);
        Piece pc = piece_on(s);


        curSt.positionalValue[color_of(pc)] += positional_value_of(pc,s);
        if (type_of(pc) == KING)
            curSt.kingLastValue[color_of(pc)] = positional_value_of(pc,s);
    }



    if (curSt.epSquare != SQ_NONE)
        curSt.key ^= Zobrist::enpassant[file_of(curSt.epSquare)];

    if (sideToMove == BLACK)
        curSt.key ^= Zobrist::side;

    curSt.key ^= Zobrist::castling[curSt.castlingRights];

}


// Returns a FEN representation of the position. This is mainly a debugging function.
string ZorbistFenBoard::fen() const {

    int emptyCnt;
    std::ostringstream ss;

    for (Rank r = RANK_8; r >= RANK_1; --r) {
        for (File f = FILE_A; f <= FILE_H; ++f) {
            for (emptyCnt = 0; f <= FILE_H && empty(make_square(f, r)); ++f)
                ++emptyCnt;

            if (emptyCnt)
                ss << emptyCnt;

            if (f <= FILE_H)
                ss << PieceToChar[piece_on(make_square(f, r))];
        }

        if (r > RANK_1)
            ss << '/';
    }

    ss << (sideToMove == WHITE ? " w " : " b ");

    if (can_castle(WHITE_OO))
        ss <<  'K';

    if (can_castle(WHITE_OOO))
        ss << 'Q';

    if (can_castle(BLACK_OO))
        ss << 'k';

    if (can_castle(BLACK_OOO))
        ss << 'q';

    if (!can_castle(ANY_CASTLING))
        ss << '-';

    ss << (ep_square() == SQ_NONE ? " - " : " x"/*+square(ep_square())TODO+*/  " ")
       << curSt.rule50 << " " << 1 + (gamePly - (sideToMove == BLACK)) / 2;

    return ss.str();
}

// Calculates st.blockersForKing[c] and st.pinners[~c],
// which store respectively the pieces preventing king of color c from being in check
// and the slider pieces of color ~c pinning pieces of color c to the king.
void ZorbistFenBoard::update_slider_blockers(Color c){

    Square ksq = square<KING>(c);

    curSt.blockersForKing[c] = 0;
    curSt.pinners[~c] = 0;

    // Snipers are sliders that attack 's' when a piece and other snipers are removed
    Bitboard snipers = ((attacks_bb<ROOK>(ksq) & pieces(QUEEN, ROOK))
                        | (attacks_bb<BISHOP>(ksq) & pieces(QUEEN, BISHOP)))
                       & pieces(~c);
    Bitboard occupancy = pieces() ^ snipers;

    while (snipers) {
        Square sniperSq = pop_lsb(snipers);
        Bitboard b = between_bb(ksq, sniperSq) & occupancy; // between_bb does not return ksq

        if (b && !more_than_one(b)) {
            curSt.blockersForKing[c] |= b; // here pieces of both colors
            if (b & pieces(c)) // being blocked by piece colored same as king
                curSt.pinners[~c] |= sniperSq;
        }
    }
}


// Computes a bitboard of all pieces which attack a given square.
// Slider attacks use the occupied bitboard to indicate occupancy.
Bitboard ZorbistFenBoard::attackers_to(Square s, Bitboard occupied) const {

    return (pawn_attacks_bb(BLACK, s) & pieces(WHITE, PAWN))
           | (pawn_attacks_bb(WHITE, s) & pieces(BLACK, PAWN))
           | (attacks_bb<KNIGHT>(s) & pieces(KNIGHT))
           | (attacks_bb<ROOK>(s, occupied) & pieces(ROOK, QUEEN))
           | (attacks_bb<BISHOP>(s, occupied) & pieces(BISHOP, QUEEN))
           | (attacks_bb<KING>(s) & pieces(KING));
}


// Flips position with the white and black sides reversed. This
// is only useful for debugging.
void ZorbistFenBoard::flip() {

    string f, token;
    std::stringstream ss(fen());

    for (Rank r = RANK_8; r >= RANK_1; --r)  // Piece placement
    {
        std::getline(ss, token, r > RANK_1 ? '/' : ' ');
        f.insert(0, token + (f.empty() ? " " : "/"));
    }

    ss >> token;                        // Active color
    f += (token == "w" ? "B " : "W ");  // Will be lowercased later

    ss >> token;  // Castling availability
    f += token + " ";

    std::transform(f.begin(), f.end(), f.begin(),
                   [](char c) { return char(islower(c) ? toupper(c) : tolower(c)); });

    ss >> token;  // En passant square
    f += (token == "-" ? token : token.replace(1, 1, token[1] == '3' ? "6" : "3"));

    std::getline(ss, token);  // Half and full moves
    f += token;

    set(f);

    assert(pos_is_ok());
}


// Performs some consistency checks for the position object
// and raise an assert if something wrong is detected.
// This is meant to be helpful when debugging.
bool ZorbistFenBoard::pos_is_ok() const {

    constexpr bool Fast = true;  // Quick (default) or full check?

    if ((sideToMove != WHITE && sideToMove != BLACK) || piece_on(square<KING>(WHITE)) != W_KING
        || piece_on(square<KING>(BLACK)) != B_KING
        || (ep_square() != SQ_NONE && relative_rank(sideToMove, ep_square()) != RANK_6))
        assert(0 && "pos_is_ok: Default");

    if (Fast)
        return true;

    if (pieceCount[W_KING] != 1 || pieceCount[B_KING] != 1
        || attackers_to(square<KING>(~sideToMove)) & pieces(sideToMove))
        assert(0 && "pos_is_ok: Kings");

    if ((pieces(PAWN) & (Rank1BB | Rank8BB)) || pieceCount[W_PAWN] > 8 || pieceCount[B_PAWN] > 8)
        assert(0 && "pos_is_ok: Pawns");

    if ((pieces(WHITE) & pieces(BLACK)) || (pieces(WHITE) | pieces(BLACK)) != pieces()
        || popcount(pieces(WHITE)) > 16 || popcount(pieces(BLACK)) > 16)
        assert(0 && "pos_is_ok: Bitboards");

    for (PieceType p1 = PAWN; p1 <= KING; ++p1)
        for (PieceType p2 = PAWN; p2 <= KING; ++p2)
            if (p1 != p2 && (pieces(p1) & pieces(p2)))
                assert(0 && "pos_is_ok: Bitboards");


    for (Piece pc: Pieces)
        if (pieceCount[pc] != popcount(pieces(color_of(pc), type_of(pc)))
            || pieceCount[pc] != std::count(board, board + SQUARE_NB, pc))
            assert(0 && "pos_is_ok: Pieces");

    for (Color c: {WHITE, BLACK})
        for (CastlingRights cr: {c & KING_SIDE, c & QUEEN_SIDE}) {
            if (!can_castle(cr))
                continue;

            if (piece_on(castlingRookSquare[cr]) != make_piece(c, ROOK)
                || castlingRightsMask[castlingRookSquare[cr]] != cr
                || (castlingRightsMask[square<KING>(c)] & cr) != cr)
                assert(0 && "pos_is_ok: Castling");
        }

    return true;
}


// phase is used to determine, if the king is weak (midgame)
// or a strong demolisher (endgame)
int ZorbistFenBoard::board_phase() const {
    int totalPhase = 32;
    int phase = totalPhase;

    phase -= count<KNIGHT>();
    phase -= count<BISHOP>();
    phase -= count<ROOK>() * 2;
    phase -= count<QUEEN>() * 4;

    return (phase * 256 + (totalPhase / 2)) / totalPhase;

    return (((material_value() < 3700) && !count<QUEEN>()) || material_value() < 2500) ? 256 : 0;
}

// positional value, inverts the color for consistency with inverted tables
// for the king there are two tables for different stages of the game.
Value ZorbistFenBoard::positional_value_of(Piece pc, Square s) const {
    if (type_of(pc) != KING) {
        return PiecePositionalValue[type_of(pc)][relative_square(~color_of(pc),s)];
    }
    else {
        Value open = PiecePositionalValue[KING][relative_square(~color_of(pc),s)];
        Value end = PiecePositionalValue[KING+1][relative_square(~color_of(pc),s)];
        int phase = board_phase();
        return ((open * (256 - phase)) + (end * phase)) / 256;
    }
}



