//
// Created by afentev on 4/3/21.
//

#ifndef PROJ_ITERATIVETABLESENGINE_H
#define PROJ_ITERATIVETABLESENGINE_H

#include <utility>
#include <functional>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <ChessRules.h>

#include <sqlite3.h>

#include "../Stuff/stuff.h"
//#include "Tables.h"

extern int mg_pawn_table[64];

extern int eg_pawn_table[64];

extern int mg_knight_table[64];

extern int eg_knight_table[64];

extern int mg_bishop_table[64];

extern int eg_bishop_table[64];

extern int mg_rook_table[64];

extern int eg_rook_table[64];

extern int mg_queen_table[64];

extern int eg_queen_table[64];

extern int mg_king_table[64];

extern int eg_king_table[64];

extern std::vector<std::vector<std::pair<int, int>>> mobility;

class IterativeTablesEngine;

extern std::set<char> upper;
extern std::set<char> lower;

extern std::map<char, int> piece;

extern int A1;
extern int H1;
extern int A8;
extern int H8;

extern int N;
extern int E;
extern int S;
extern int W;

//std::map<char, std::vector<int>> pstT {
//        {'P', std::vector<int> {   0,   0,   0,   0,   0,   0,   0,   0,
//        78,  83,  86,  73, 102,  82,  85,  90,
//        7,  29,  21,  44,  40,  31,  44,   7,
//        -17,  16,  -2,  15,  14,   0,  15, -13,
//        -26,   3,  10,   9,   6,   1,   0, -23,
//        -22,   9,   5, -11, -10,  -2,   3, -19,
//        -31,   8,  -7, -37, -36, -14,   3, -31,
//        0,   0,   0,   0,   0,   0,   0,   0}},
//        {'N', std::vector<int> { -66, -53, -75, -75, -10, -55, -58, -70,
//        -3,  -6, 100, -36,   4,  62,  -4, -14,
//        10,  67,   1,  74,  73,  27,  62,  -2,
//        24,  24,  45,  37,  33,  41,  25,  17,
//        -1,   5,  31,  21,  22,  35,   2,   0,
//        -18,  10,  13,  22,  18,  15,  11, -14,
//        -23, -15,   2,   0,   2,   0, -23, -20,
//        -74, -23, -26, -24, -19, -35, -22, -69}},
//        {'B', std::vector<int>{ -59, -78, -82, -76, -23,-107, -37, -50,
//        -11,  20,  35, -42, -39,  31,   2, -22,
//        -9,  39, -32,  41,  52, -10,  28, -14,
//        25,  17,  20,  34,  26,  25,  15,  10,
//        13,  10,  17,  23,  17,  16,   0,   7,
//        14,  25,  24,  15,   8,  25,  20,  15,
//        19,  20,  11,   6,   7,   6,  20,  16,
//        -7,   2, -15, -12, -14, -15, -10, -10}},
//        {'R', std::vector<int>{  35,  29,  33,   4,  37,  33,  56,  50,
//        55,  29,  56,  67,  55,  62,  34,  60,
//        19,  35,  28,  33,  45,  27,  25,  15,
//        0,   5,  16,  13,  18,  -4,  -9,  -6,
//        -28, -35, -16, -21, -13, -29, -46, -30,
//        -42, -28, -42, -25, -25, -35, -26, -46,
//        -53, -38, -31, -26, -29, -43, -44, -53,
//        -30, -24, -18,   5,  -2, -18, -31, -32}},
//        {'Q', std::vector<int>{   6,   1,  -8,-104,  69,  24,  88,  26,
//        14,  32,  60, -10,  20,  76,  57,  24,
//        -2,  43,  32,  60,  72,  63,  43,   2,
//        1, -16,  22,  17,  25,  20, -13,  -6,
//        -14, -15,  -2,  -5,  -1, -10, -20, -22,
//        -30,  -6, -13, -11, -16, -11, -16, -27,
//        -36, -18,   0, -19, -15, -15, -21, -38,
//        -39, -30, -31, -13, -31, -36, -34, -42}},
//        {'K', std::vector<int>{   4,  54,  47, -99, -99,  60,  83, -62,
//        -32,  10,  55,  56,  56,  55,  10,   3,
//        -62,  12, -57,  44, -67,  28,  37, -31,
//        -55,  50,  11,  -4, -19,  13,   0, -49,
//        -55, -43, -52, -28, -51, -47,  -8, -50,
//        -47, -42, -43, -79, -64, -32, -29, -32,
//        -4,   3, -14, -50, -57, -18,  13,   4,
//        17,  30,  -3, -14,   6,  -1,  40,  18}},
//};

extern std::map<char, std::vector<int>> pstTMG;

extern std::map<char, std::vector<int>> pstTEG;

extern std::map<char, std::vector<int>> directions;

extern int MATE_LOWER;
extern int MATE_UPPER;

extern int TABLE_SIZE;
extern int QS_LIMIT;
extern int EVAL_ROUGHNES;
extern bool DRAW_TEST;

extern std::string initial;


class IterativeTablesEngine {
public:
    std::string board;
    int score = 0;
    std::pair<bool, bool> whiteCastle, blackCastle;
    int ep{}, kp{};
    std::unordered_map<std::string, int> hist;
    double overallScore = 0;
    int figures = 16;
    bool AIBlack;

    bool operator== (const IterativeTablesEngine& other) const;

    bool operator< (const IterativeTablesEngine& other) const;

    IterativeTablesEngine();

    IterativeTablesEngine(std::string b, int sc, std::pair<bool, bool> wc, std::pair<bool, bool> bc,
                          int ep, int kp, std::unordered_map<std::string, int> h, double ovsc, int f, bool);

    [[nodiscard]] std::vector<std::pair<std::pair<int, int>, int>> genMoves() const;

    [[nodiscard]] IterativeTablesEngine rotate() const;

    [[nodiscard]] IterativeTablesEngine nullMove() const;

    IterativeTablesEngine move(std::pair<int, int> move) const;

    static inline int getTable(char p, int ind, double rate);

    int value(std::pair<int, int> move) const;
};

struct less_than_key {
public:
    IterativeTablesEngine e;
    std::map<std::pair<int, int>, int> mem;

    explicit less_than_key(IterativeTablesEngine e);

    inline bool operator() (const std::pair<std::pair<int, int>, int>& struct1,
                            const std::pair<std::pair<int, int>, int>& struct2);
};

class Searcher {
public:
    int nodes = 0;
    std::map<std::tuple<IterativeTablesEngine, int, bool>, std::pair<int, int>> tp_score;
    std::map<IterativeTablesEngine, std::pair<int, int>> tp_move;
    std::set<IterativeTablesEngine> history;

    static bool is_dead(const IterativeTablesEngine& pos);

    int bound(IterativeTablesEngine pos, double gamma, int depth, bool root = true);

    std::tuple<int, std::pair<int, int>, int> search(const IterativeTablesEngine& pos,
                                                     const std::vector<IterativeTablesEngine>& h,
                                                     int);
};

void printPos(IterativeTablesEngine pos);

int parse(std::string s, bool aiblack, bool mode = true, bool aiTurn = false);

std::string backparse(int x, bool AIMove = false);

std::string reverse(const std::string& s);

void fillTables(bool aiblack);
//void go();

std::string getMove(thc::ChessRules* rules, const IterativeTablesEngine& eng, bool, bool);

//void play();


extern std::string mv1, mv2;
extern sqlite3* db;
extern char *zErrMsg;

std::string openingDB(const std::string& pos);

std::string endingDB(const std::string& pos);

std::string getMove(thc::ChessRules* rules, const IterativeTablesEngine& eng);

#endif //PROJ_ITERATIVETABLESENGINE_H
