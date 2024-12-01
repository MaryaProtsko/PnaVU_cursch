#include "bitboard.h"

#include <algorithm>
#include <bitset>

uint8_t SquareDistance[SQUARE_NB][SQUARE_NB];

Bitboard LineBB[SQUARE_NB][SQUARE_NB];
Bitboard BetweenBB[SQUARE_NB][SQUARE_NB];
Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];


// alignas for cpu operations
alignas(64) Magic Magics[SQUARE_NB][2];

namespace {

    Bitboard RookTable[0x19000];   // To store rook attacks
    Bitboard BishopTable[0x1480];  // To store bishop attacks

    void init_magics(PieceType pt, Bitboard table[], Magic magics[][2]);

// Returns the bitboard of target square for the given step
// from the given square. If the step is off the board, returns empty bitboard.
    Bitboard safe_destination(Square s, int step) {
        Square to = Square(s + step);
        return is_ok(to) && distance(s, to) <= 2 ? square_bb(to) : Bitboard(0);
    }
}

// Returns an ASCII representation of a bitboard suitable
// to be printed to standard output. Useful for debugging.
std::string Bitboards::pretty(Bitboard b) {

    std::string s = "+---+---+---+---+---+---+---+---+\n";

    for (Rank r = RANK_8; r >= RANK_1; --r) {
        for (File f = FILE_A; f <= FILE_H; ++f)
            s += b & make_square(f, r) ? "| X " : "|   ";

        s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+\n";
    }
    s += "  a   b   c   d   e   f   g   h\n";

    return s;
}


// Initializes various bitboard tables. It is called at
// startup
void Bitboards::init() {

    for (Square s1 = SQ_A1; s1 <= SQ_H8; ++s1)
        for (Square s2 = SQ_A1; s2 <= SQ_H8; ++s2)
            SquareDistance[s1][s2] = std::max(distance<File>(s1, s2), distance<Rank>(s1, s2));

    init_magics(ROOK, RookTable, Magics);
    init_magics(BISHOP, BishopTable, Magics);

    for (Square s1 = SQ_A1; s1 <= SQ_H8; ++s1) {
        PawnAttacks[WHITE][s1] = pawn_attacks_bb<WHITE>(square_bb(s1));
        PawnAttacks[BLACK][s1] = pawn_attacks_bb<BLACK>(square_bb(s1));

        for (int step: {-9, -8, -7, -1, 1, 7, 8, 9})
            PseudoAttacks[KING][s1] |= safe_destination(s1, step);

        for (int step: {-17, -15, -10, -6, 6, 10, 15, 17})
            PseudoAttacks[KNIGHT][s1] |= safe_destination(s1, step);

        PseudoAttacks[QUEEN][s1] = PseudoAttacks[BISHOP][s1] = attacks_bb<BISHOP>(s1, 0);
        PseudoAttacks[QUEEN][s1] |= PseudoAttacks[ROOK][s1] = attacks_bb<ROOK>(s1, 0);

        for (PieceType pt: {BISHOP, ROOK})
            for (Square s2 = SQ_A1; s2 <= SQ_H8; ++s2) {
                if (PseudoAttacks[pt][s1] & s2) {
                    LineBB[s1][s2] = (attacks_bb(pt, s1, 0) & attacks_bb(pt, s2, 0)) | s1 | s2;
                    BetweenBB[s1][s2] =
                            (attacks_bb(pt, s1, square_bb(s2)) & attacks_bb(pt, s2, square_bb(s1)));
                }
                BetweenBB[s1][s2] |= s2;
            }
    }
}

namespace {

    Bitboard sliding_attack(PieceType pt, Square sq, Bitboard occupied) {

        Bitboard attacks = 0;
        Direction RookDirections[4] = {NORTH, SOUTH, EAST, WEST};
        Direction BishopDirections[4] = {NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST};

        for (Direction d: (pt == ROOK ? RookDirections : BishopDirections)) {
            Square s = sq;
            while (safe_destination(s, d)) { // just looping to border
                attacks |= (s += d);
                if (occupied & s) {
                    break;
                }
            }
        }

        return attacks;
    }


// Computes all rook and bishop attacks at startup. Magic
// bitboards are used to look up attacks of sliding pieces. As a reference see
// https://www.chessprogramming.org/Magic_Bitboards. In particular, here
// the so called "fancy" approach is being used.
    void init_magics(PieceType pt, Bitboard table[], Magic magics[][2]) {
        int size = 0;

        for (Square s = SQ_A1; s <= SQ_H8; ++s) {
            // Board edges are not considered in the relevant occupancies
            Bitboard edges = ((Rank1BB | Rank8BB) & ~rank_bb(s)) | ((FileABB | FileHBB) & ~file_bb(s));

            // Given a square 's', the mask is the bitboard of sliding attacks from
            // 's' computed on an empty board. The index must be big enough to contain
            // all the attacks for each possible subset of the mask and so is 2 power
            // the number of 1s of the mask.
            Magic &m = magics[s][pt - BISHOP];
            m.mask = sliding_attack(pt, s, 0) & ~edges;

            // Set the offset for the attacks table of the square. We have individual
            // table sizes for each square with "Fancy Magic Bitboards".

            m.attacks = s == SQ_A1 ? table : magics[s - 1][pt - BISHOP].attacks + size;
            size = 0;

            // Use Carry-Rippler trick to enumerate all subsets of masks[s] and
            // store the corresponding sliding attack bitboard in reference[].
            // https://www.chessprogramming.org/Traversing_Subsets_of_a_Set
            Bitboard b = 0;
            do {
                m.attacks[_pext_u64(b, m.mask)] = sliding_attack(pt, s, b);
                size++;
                b = (b - m.mask) & m.mask;
            } while (b);
        }
    }
}
