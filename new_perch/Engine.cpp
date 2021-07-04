//
// Created by afentev on 6/4/21.
//

#include "Engine.h"

#include <utility>

std::vector<std::vector<int>> MVVLVA;

std::tuple<int, Move, int> Engine::search(const Position &pos,
                                          const std::vector<Position> &h,
                                          int time_control) noexcept {
    nodes = 0;
    if (DRAW_TEST) {
        this->history = std::unordered_set<Position>();
        for (const auto& x : h) {
            this->history.insert(x);
        }
        this->tp_score.clear();
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int depth = 1; depth < 50; ++depth) {
//        std::cout << depth << std::endl;
        int lw = -MATE_UPPER;
        int up = MATE_UPPER;
        vertexes = 0;
        observed = 0;
        while (lw < up - EVAL_ROUGHNESS) {
            int gamma = (lw + up + 1) / 2;
            int score = bound(pos, gamma, depth);
            std::cout << depth << ' ' << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() << std::endl;
//                std::cout << lw << ' ' << up << ' ' << gamma << ' ' << score << std::endl;
            if (score >= gamma) {
                lw = score;
            }
            if (score < gamma) {
                up = score;
            }
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() > time_control) {
            std::cout << static_cast<double>(observed) / vertexes << std::endl;
            std::cout << depth << std::endl;
            bound(pos, lw, depth);
            return std::make_tuple(depth, tp_move[pos], tp_score[std::make_tuple(pos, depth, true)].first);
        }
    }
}

int Engine::bound(Position pos, double gamma, int depth, bool root) noexcept {
    vertexes++;
    observed++;

    this->nodes++;
    depth = depth > 0? depth: 0;

    if (pos.score <= -MATE_LOWER) {
        vertexes--;
        return -MATE_UPPER;
    }

//    if (DRAW_TEST) {
        if (!root) {
            if (history.contains(pos)) {
                vertexes--;
                return 0;
            }
        }
//    }

    std::pair<int, int> entry;
    auto t = std::make_tuple(pos, depth, root);
    if (!tp_score.contains(t)) {
        entry = {-MATE_UPPER, MATE_UPPER};
    } else {
        entry = tp_score.at(t);
    }

    if (entry.first >= gamma && (!root || tp_move.contains(pos))) {
        vertexes--;
        return entry.first;
    }
    if (entry.second < gamma) {
        vertexes--;
        return entry.second;
    }

    int best = -MATE_UPPER;
    bool rbnq = (pos.occupied & ~(pos.whitePawns | pos.blackPawns | pos.whiteKings | pos.blackKings)) != 0;

    std::vector<std::pair<Move, int>> moves;
    if (depth > 0 && !root && rbnq) {
        auto res = -bound(pos.nullMove(), 1 - gamma,
                          depth - 3, false);
        best = best > res? best: res;

        if (res >= gamma && pos.score > -MATE_LOWER) {
            if (tp_move.size() > TABLE_SIZE) {
                tp_move.clear();
            }
            tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
            vertexes--;
            return res;
        }

        moves.emplace_back(Move(), res);
    }
    if (depth == 0) {
        auto res = pos.score;
        best = best > res? best: res;

        if (res >= gamma) {
            if (tp_move.size() > TABLE_SIZE) {
                tp_move.clear();
            }
            tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
            vertexes--;
            return res;
        }

        moves.emplace_back(Move(), res);
    }

    if (tp_move.contains(pos)) {
        auto killer = tp_move.at(pos);
        if (depth > 0 || pos.value(killer) >= QS_LIMIT) {
            auto res = -bound(pos.playMoveCopy(killer, pos.white2play),
                              1 - gamma, depth - 1, false);
            if (res >= gamma) {
                if (tp_move.size() > TABLE_SIZE) {
                    tp_move.clear();
                }
                if (killer.src != 0xFFFFFFFFFFFFFFFF) {
                    tp_move[pos] = killer;
                }
                tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
                return res;
            }
            best = best > res? best: res;
            moves.emplace_back(killer, res);
        }
    }

    auto x = pos.moves;
//    std::cout << x.size() << std::endl;
    std::sort(x.begin(), x.end(), less_than_key());

    int count = 0;
    for (auto move : x) {
        int value = move.second;
        if (depth > 0 || value >= QS_LIMIT) {
            int res;
            if (count >= 5 && !move.first.isCapture && depth >= 3 && value < gamma) {
                res = -bound(pos.playMoveCopy(move.first, pos.white2play),
                             1 - gamma, depth - 2, false);
            } else {
                res = -bound(pos.playMoveCopy(move.first, pos.white2play),
                                  1 - gamma, depth - 1, false);
            }
            if (res >= gamma) {
                if (tp_move.size() > TABLE_SIZE) {
                    tp_move.clear();
                }
                if (move.first.src != 0xFFFFFFFFFFFFFFFF) {
                    tp_move[pos] = move.first;
                }
                tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
                return res;
            }
            best = best > res? best: res;
            moves.emplace_back(move.first, res);
        }
        count++;
    }

    if (best < gamma && best < 0 && depth > 0) {
        bool ok = true;
        for (auto m: x) {
            auto temporary = pos.playMoveCopy(m.first, pos.white2play);
            if (!is_dead(temporary)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            auto temporary = pos.nullMove();
            best = is_dead(temporary)? -MATE_UPPER: 0;
        }
    }

    if (tp_score.size() > TABLE_SIZE) {
        tp_score.clear();
    }
    if (best >= gamma) {
        tp_score[std::make_tuple(pos, depth, root)] = {best, entry.second};
    }
    if (best < gamma) {
        tp_score[std::make_tuple(pos, depth, root)] = {entry.first, best};
    }
    return best;
}

bool Engine::is_dead(Position& pos) const noexcept {
    for (auto m: pos.moves) {
        int value = m.second;
        if (value >= MATE_LOWER) {
            return true;
        }
    }
    return false;
}

bool less_than_key::operator()(const std::pair<Move, int> &struct1, const std::pair<Move, int> &struct2) {
    return struct1.second > struct2.second;
//    return MVVLVA[struct1.first.dst][struct1.first.src] > MVVLVA[struct2.first.dst][struct2.first.src];
    Move mv1 = struct1.first;
    Move mv2 = struct2.first;
    if (mv1.isCapture) {
        if (mv2.isCapture) {
            char source1 = board.board[mv1.src];
            char source2 = board.board[mv2.src];
            char target1 = board.board[mv1.dst];
            char target2 = board.board[mv2.dst];

            return MVVLVA[target1][source1] > MVVLVA[target2][source2];
        }
        return true;
    } else if (mv2.isCapture) {
        return false;
    } else {
        return struct1.second > struct2.second;
    }
}

std::pair<int, Move> ABP::alphaBeta(Position board, int depth, int alpha, int beta) {
    int value;
    if(depth == 0 || board.isMate) {
        value = board.score;
        return {value, Move()};
    }
    int best = -MATE_UPPER;
    Position nextBoard;
    Move bestMove;

    less_than_key comparator;
    comparator.board = board;
    std::sort(board.moves.begin(), board.moves.end(), comparator);
    for (int i = 0; i < board.moves.size(); ++i) {
        auto move = board.moves[i].first;
        nextBoard = board.playMoveCopy(move, board.white2play);
        value = -alphaBeta(nextBoard, depth-1,-beta,-alpha).first;
        if (value > best) {
            bestMove = move;
            best = value;
        }
        if (best > alpha) {
            alpha = best;
        }
        if (best >= beta) {
            break;
        }
    }
    return {best, bestMove};
}

std::pair<int, Move> ABP::alphaBetaTT(Position board, int depth, int alpha, int beta) {
    int value;
    auto hash = hasher(board);
    if (TT.contains(hash)) {
        auto tte = TT[hash];
        int depthTT = std::get<2>(tte);
        if (depthTT >= depth) {
            int val = std::get<0>(tte);
            int type = std::get<1>(tte);

            if (type == 0) {// stored value is exact return tte.value;
                return {val, std::get<3>(tte)};
            }
            if(type == -1 && val > alpha) {
                alpha = val;           // update lowerbound alpha if needed
            } else if(type == 1 && val < beta) {
                beta = val;            // update upperbound beta if needed
            }
            if(alpha >= beta) {
                return {val, std::get<3>(tte)};            // if lowerbound surpasses upperbound
            }
        }
    }
    if(depth == 0 || board.isMate) {
        value = board.score;
        if(value <= alpha) { // a lowerbound value
            TT[hasher(board)] = {value, -1, depth, Move()};
        } else if(value >= beta) {        // an upperbound value
            TT[hasher(board)] = {value, 1, depth, Move()};
        } else {                          // a true minimax value
            TT[hasher(board)] = {value, 0, depth, Move()};
        }
        return {value, Move()};
    }
    int best = -MATE_UPPER-1;
    Move move;
    Move bestMove;
    Position nextBoard;
    less_than_key comparator;
    comparator.board = board;

    bool rbnq = (board.occupied & ~(board.whitePawns | board.blackPawns | board.whiteKings | board.blackKings)) != 0;
    if (depth >= 3 && rbnq) {
        nextBoard = board.nullMove();
        value = -alphaBetaTT(nextBoard, depth - 3, -beta, -alpha).first;
        if (value > best) {
            bestMove = move;
            best = value;
        }
        if (best > alpha) {
            alpha = best;
        }
    }
    if (best < alpha) {
        std::sort(board.moves.begin(), board.moves.end(), comparator);

        for (int i = 0; i < board.moves.size(); ++i) {
            move = board.moves[i].first;
            nextBoard = board.playMoveCopy(move, board.white2play);
            if (i >= 5 && !move.isCapture && board.moves[i].second < alpha) {
                value = -alphaBetaTT(nextBoard, depth - 2, -beta, -alpha).first;
            } else {
                value = -alphaBetaTT(nextBoard, depth - 1, -beta, -alpha).first;
            }
            if (value > best) {
                bestMove = move;
                best = value;
            }
            if (best > alpha) {
                alpha = best;
            }
            if (best >= beta) {
                break;
            }
        }
    }
    if(best <= alpha) { // a lowerbound value
        TT[hasher(board)] = {best, -1, depth, bestMove};
    } else if(best >= beta) {        // an upperbound value
        TT[hasher(board)] = {best, 1, depth, bestMove};
    } else {
        // a true minimax value
        TT[hasher(board)] = {best, 0, depth, bestMove};
    }
    return {best, bestMove};
}

Move ABP::getMove(Position board) {
    TT.clear();
    TT.reserve(1e8);

    std::cout << "Begin" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto res_ = alphaBetaTT(board, 10, -MATE_UPPER, MATE_UPPER);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    res_.first;

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;

    begin = std::chrono::steady_clock::now();
    auto res = alphaBeta(std::move(board), 10, -MATE_UPPER, MATE_UPPER);
    end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
    std::cout << "End" << std::endl << std::endl;

    return res.second;
}

ABP::ABP() {
    MVVLVA.resize(122);
    for (int i = 0; i < 122; ++i) {
        int def = 0;
        if (i == 'K' || i == 'k') {
            def = 50;
        } else if (i == 'Q' || i == 'q') {
            def = 40;
        } else if (i == 'R' || i == 'r') {
            def = 30;
        } else if (i == 'B' || i == 'N' || i == 'b' || i == 'n') {
            def = 20;
        } else if (i == 'P' || i == 'p' || i == ' ') {
            def = 10;
        }

        MVVLVA[i].resize(122, def);
        for (int j = 0; j < 122; ++j) {
            if (j == 'K' || j == 'k') {
                MVVLVA[i][j] += 1;
            } else if (j == 'Q' || j == 'q') {
                MVVLVA[i][j] += 2;
            } else if (j == 'R' || j == 'r') {
                MVVLVA[i][j] += 3;
            } else if (j == 'B' || j == 'N' || j == 'b' || j == 'n') {
                MVVLVA[i][j] += 4;
            } else if (j == 'P' || j == 'p') {
                MVVLVA[i][j] += 5;
            }
        }
    }
}

int count = 0;

int Perft(int depth, Position pos) {
    count++;
    if (pos.isMate) {
        return 1ull;
    }
    auto move_list = pos.moves;
    int n_moves, i;
    int nodes = 0;

    if (depth == 0) {
        return 1ULL;
    }

    auto moves = pos.genMovesFast();
    n_moves = moves.size();
    for (i = 0; i < n_moves; i++) {
        nodes += Perft(depth - 1, pos.playMoveCopy(moves[i], pos.white2play));
    }
    return nodes;
}
