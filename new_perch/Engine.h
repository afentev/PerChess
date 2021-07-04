//
// Created by afentev on 6/4/21.
//

#ifndef PERCHESS_ENGINE_H
#define PERCHESS_ENGINE_H

#include <chrono>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <future>

#include "Position.h"

const int TABLE_SIZE = 7e6;
const int QS_LIMIT = 219;
const int EVAL_ROUGHNESS = 13;
const bool DRAW_TEST = true;
const int MATE_LOWER = 100000 - 50 * 10;
const int MATE_UPPER = 100000 + 50 * 10;

extern std::vector<std::vector<int>> MVVLVA;

class Engine {
public:
    Engine() {
        tp_score.reserve(1e8);
        tp_move.reserve(1e8);
//        history.reserve(1e7);
        tp_score.max_load_factor(0.1);
        tp_move.max_load_factor(0.1);
//        history.max_load_factor(0.2);
    }

    int observed = 0;
    int vertexes = 0;

    int nodes = 0;
    std::unordered_map<std::tuple<Position, int, bool>, std::pair<int, int>, key_hash> tp_score;
    std::unordered_map<Position, Move> tp_move;
    std::unordered_set<Position> history;

    std::tuple<int, Move, int> search(const Position& pos,
                                      const std::vector<Position>& h,
                                      int time_control) noexcept;

    int bound(Position pos, double gamma, int depth, bool root = true) noexcept;

    bool is_dead(Position& pos) const noexcept;
};

struct less_than_key {
public:
    Position board;
    inline bool operator() (const std::pair<Move, int>& struct1,
                            const std::pair<Move, int>& struct2);
};

class ABP {
public:
    ABP();

    std::hash<Position> hasher;
    std::unordered_map<size_t, std::tuple<int, int, int, Move>> TT;
    Move getMove(Position board);
    std::pair<int, Move> alphaBeta(Position board, int depth, int alpha, int beta);
    std::pair<int, Move> alphaBetaTT(Position board, int depth, int alpha, int beta);
};

int Perft(int depth, Position p);

extern int count;


#endif //PERCHESS_ENGINE_H
