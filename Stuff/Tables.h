//
// Created by afentev on 3/25/21.
//

#ifndef PROJ_TABLES_H
#define PROJ_TABLES_H

#define PAWN   0
#define KNIGHT 1
#define BISHOP 2
#define ROOK   3
#define QUEEN  4
#define KING   5

/* board representation */
#define WHITE  0
#define BLACK  1

#define WHITE_PAWN      (2*PAWN   + WHITE)
#define BLACK_PAWN      (2*PAWN   + BLACK)
#define WHITE_KNIGHT    (2*KNIGHT + WHITE)
#define BLACK_KNIGHT    (2*KNIGHT + BLACK)
#define WHITE_BISHOP    (2*BISHOP + WHITE)
#define BLACK_BISHOP    (2*BISHOP + BLACK)
#define WHITE_ROOK      (2*ROOK   + WHITE)
#define BLACK_ROOK      (2*ROOK   + BLACK)
#define WHITE_QUEEN     (2*QUEEN  + WHITE)
#define BLACK_QUEEN     (2*QUEEN  + BLACK)
#define WHITE_KING      (2*KING   + WHITE)
#define BLACK_KING      (2*KING   + BLACK)
#define EMPTY           (BLACK_KING  +  1)

#define PCOLOR(p) ((p)&1)

int side2move;
int board[64];

#define FLIP(sq) ((sq)^56)
#define OTHER(side) ((side)^ 1)

int mg_value[6] = { 82, 337, 365, 477, 1025,  0};
int eg_value[6] = { 94, 281, 297, 512,  936,  0};

/* piece/sq tables */
/* values from Rofchade: http://www.talkchess.com/forum3/viewtopic.php?f=2&t=68311&start=19 */

int mg_pawn_table[64] = {
        0,   0,   0,   0,   0,   0,  0,   0,
        98, 134,  61,  95,  68, 126, 34, -11,
        -6,   7,  26,  31,  65,  56, 25, -20,
        -14,  13,   6,  21,  23,  12, 17, -23,
        -27,  -2,  -5,  12,  17,   6, 10, -25,
        -26,  -4,  -4, -10,   3,   3, 33, -12,
        -35,  -1, -20, -23, -15,  24, 38, -22,
        0,   0,   0,   0,   0,   0,  0,   0,
};

int eg_pawn_table[64] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        178, 173, 158, 134, 147, 132, 165, 187,
        94, 100,  85,  67,  56,  53,  82,  84,
        32,  24,  13,   5,  -2,   4,  17,  17,
        13,   9,  -3,  -7,  -7,  -8,   3,  -1,
        4,   7,  -6,   1,   0,  -5,  -1,  -8,
        13,   8,   8,  10,  13,   0,   2,  -7,
        0,   0,   0,   0,   0,   0,   0,   0,
};

int mg_knight_table[64] = {
        -167, -89, -34, -49,  61, -97, -15, -107,
        -73, -41,  72,  36,  23,  62,   7,  -17,
        -47,  60,  37,  65,  84, 129,  73,   44,
        -9,  17,  19,  53,  37,  69,  18,   22,
        -13,   4,  16,  13,  28,  19,  21,   -8,
        -23,  -9,  12,  10,  19,  17,  25,  -16,
        -29, -53, -12,  -3,  -1,  18, -14,  -19,
        -105, -21, -58, -33, -17, -28, -19,  -23,
};

int eg_knight_table[64] = {
        -58, -38, -13, -28, -31, -27, -63, -99,
        -25,  -8, -25,  -2,  -9, -25, -24, -52,
        -24, -20,  10,   9,  -1,  -9, -19, -41,
        -17,   3,  22,  22,  22,  11,   8, -18,
        -18,  -6,  16,  25,  16,  17,   4, -18,
        -23,  -3,  -1,  15,  10,  -3, -20, -22,
        -42, -20, -10,  -5,  -2, -20, -23, -44,
        -29, -51, -23, -15, -22, -18, -50, -64,
};

int mg_bishop_table[64] = {
        -29,   4, -82, -37, -25, -42,   7,  -8,
        -26,  16, -18, -13,  30,  59,  18, -47,
        -16,  37,  43,  40,  35,  50,  37,  -2,
        -4,   5,  19,  50,  37,  37,   7,  -2,
        -6,  13,  13,  26,  34,  12,  10,   4,
        0,  15,  15,  15,  14,  27,  18,  10,
        4,  15,  16,   0,   7,  21,  33,   1,
        -33,  -3, -14, -21, -13, -12, -39, -21,
};

int eg_bishop_table[64] = {
        -14, -21, -11,  -8, -7,  -9, -17, -24,
        -8,  -4,   7, -12, -3, -13,  -4, -14,
        2,  -8,   0,  -1, -2,   6,   0,   4,
        -3,   9,  12,   9, 14,  10,   3,   2,
        -6,   3,  13,  19,  7,  10,  -3,  -9,
        -12,  -3,   8,  10, 13,   3,  -7, -15,
        -14, -18,  -7,  -1,  4,  -9, -15, -27,
        -23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int mg_rook_table[64] = {
        32,  42,  32,  51, 63,  9,  31,  43,
        27,  32,  58,  62, 80, 67,  26,  44,
        -5,  19,  26,  36, 17, 45,  61,  16,
        -24, -11,   7,  26, 24, 35,  -8, -20,
        -36, -26, -12,  -1,  9, -7,   6, -23,
        -45, -25, -16, -17,  3,  0,  -5, -33,
        -44, -16, -20,  -9, -1, 11,  -6, -71,
        -19, -13,   1,  17, 16,  7, -37, -26,
};

int eg_rook_table[64] = {
        13, 10, 18, 15, 12,  12,   8,   5,
        11, 13, 13, 11, -3,   3,   8,   3,
        7,  7,  7,  5,  4,  -3,  -5,  -3,
        4,  3, 13,  1,  2,   1,  -1,   2,
        3,  5,  8,  4, -5,  -6,  -8, -11,
        -4,  0, -5, -1, -7, -12,  -8, -16,
        -6, -6,  0,  2, -9,  -9, -11,  -3,
        -9,  2,  3, -1, -5, -13,   4, -20,
};

int mg_queen_table[64] = {
        -28,   0,  29,  12,  59,  44,  43,  45,
        -24, -39,  -5,   1, -16,  57,  28,  54,
        -13, -17,   7,   8,  29,  56,  47,  57,
        -27, -27, -16, -16,  -1,  17,  -2,   1,
        -9, -26,  -9, -10,  -2,  -4,   3,  -3,
        -14,   2, -11,  -2,  -5,   2,  14,   5,
        -35,  -8,  11,   2,   8,  15,  -3,   1,
        -1, -18,  -9,  10, -15, -25, -31, -50,
};

int eg_queen_table[64] = {
        -9,  22,  22,  27,  27,  19,  10,  20,
        -17,  20,  32,  41,  58,  25,  30,   0,
        -20,   6,   9,  49,  47,  35,  19,   9,
        3,  22,  24,  45,  57,  40,  57,  36,
        -18,  28,  19,  47,  31,  34,  39,  23,
        -16, -27,  15,   6,   9,  17,  10,   5,
        -22, -23, -30, -16, -16, -23, -36, -32,
        -33, -28, -22, -43,  -5, -32, -20, -41,
};

int mg_king_table[64] = {
        -65,  23,  16, -15, -56, -34,   2,  13,
        29,  -1, -20,  -7,  -8,  -4, -38, -29,
        -9,  24,   2, -16, -20,   6,  22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49,  -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
        1,   7,  -8, -64, -43, -16,   9,   8,
        -15,  36,  12, -54,   8, -28,  24,  14,
};

int eg_king_table[64] = {
        -74, -35, -18, -18, -11,  15,   4, -17,
        -12,  17,  14,  17,  17,  38,  23,  11,
        10,  17,  23,  15,  20,  45,  44,  13,
        -8,  22,  24,  27,  26,  33,  26,   3,
        -18,  -4,  21,  24,  27,  23,   9, -11,
        -19,  -3,  11,  21,  23,  16,   7,  -9,
        -27, -11,   4,  13,  14,   4,  -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43
};

int* mg_pesto_table[6] =
        {
                mg_pawn_table,
                mg_knight_table,
                mg_bishop_table,
                mg_rook_table,
                mg_queen_table,
                mg_king_table
        };

int* eg_pesto_table[6] =
        {
                eg_pawn_table,
                eg_knight_table,
                eg_bishop_table,
                eg_rook_table,
                eg_queen_table,
                eg_king_table
        };

int gamephaseInc[12] = {0,0,1,1,1,1,2,2,4,4,0,0};
int mg_table[12][64];
int eg_table[12][64];

void init_tables() {
    int pc, p, sq;
    for (p = PAWN, pc = WHITE_PAWN; p <= KING; pc += 2, p++) {
        for (sq = 0; sq < 64; sq++) {
            mg_table[pc]  [sq] = mg_value[p] + mg_pesto_table[p][sq];
            eg_table[pc]  [sq] = eg_value[p] + eg_pesto_table[p][sq];
            mg_table[pc+1][sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
            eg_table[pc+1][sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
        }
    }
}

int eval()
{
    int mg[2];
    int eg[2];
    int gamePhase = 0;

    mg[WHITE] = 0;
    mg[BLACK] = 0;
    eg[WHITE] = 0;
    eg[BLACK] = 0;

    /* evaluate each piece */
    for (int sq = 0; sq < 64; ++sq) {
        int pc = board[sq];
        if (pc != EMPTY) {
            mg[PCOLOR(pc)] += mg_table[pc][sq];
            eg[PCOLOR(pc)] += eg_table[pc][sq];
            gamePhase += gamephaseInc[pc];
        }
    }

    /* tapered eval */
    int mgScore = mg[side2move] - mg[OTHER(side2move)];
    int egScore = eg[side2move] - eg[OTHER(side2move)];
    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;
    return (mgScore * mgPhase + egScore * egPhase) / 24;
}

std::vector<std::vector<std::pair<int, int>>> mobility {
        { {-38,-33}, {-25,-23}, {-12,-13}, { 0, -3}, {12,  7},
                {25, 17}, { 31, 22}, { 38, 27}, { 38, 27} },  // Knights


        { {-25,-30}, {-11,-16}, {  3, -2}, {17, 12}, {31, 26},
                {45, 40}, { 57, 52}, { 65, 60}, { 71, 65}, {74, 69},
                {76, 71}, {78, 73}, { 79, 74}, { 80, 75}, { 81, 76},
                {81, 76} },  // Bishops


        { {-20,-36}, {-14,-19}, { -8, -3}, {-2, 13}, { 4, 29},
                {10, 46}, { 14, 62}, { 19, 79}, { 23, 95}, {26,106},
                {27,111}, {28,114}, { 29,116}, { 30,117}, { 31,118},
                {32,118} },  // Rooks


        { {-10,-18}, { -8,-13}, { -6, -7}, {-3, -2}, {-1,  3},
                { 1,  8}, {  3, 13}, {  5, 19}, {  8, 23}, {10, 27},
                {12, 32}, {15, 34}, { 16, 35}, { 17, 35}, { 18, 35},
                {20, 35}, {20, 35}, {20, 35}, { 20, 35}, { 20, 35},
                { 20, 35}, {20, 35}, {20, 35}, {20, 35}, { 20, 35},
                { 20, 35}, { 20, 35}, {20, 35}, {20, 35}, {20, 35},
                { 20, 35}, { 20, 35} }  // Queens
};


std::vector<std::vector<std::pair<int, int>>> ThreatBonus {
  {
      {0, 0}, { 7, 39}, { 0,  0},
      {24, 49}, {41,100}, {41,100} }, // KNIGHT

  {
      {0, 0}, { 7, 39}, {24, 49},
      { 0,  0}, {41,100}, {41,100} }, // BISHOP

  {
      {0, 0}, {-1, 29}, {15, 49},
      {15, 49}, { 0,  0}, {24, 49} }, // ROOK

  {
      {0, 0}, {15, 39}, {15, 39},
      {15, 39}, {15, 39}, { 0,  0} }  // QUEEN
};


std::vector<std::pair<int, int>> ThreatenedByPawnPenalty = {
        {0, 0}, {0, 0}, {56, 70},
        {56, 70}, {76, 99}, {86, 118}
};

#endif //PROJ_TABLES_H
