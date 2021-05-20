//
// Created by afentev on 4/3/21.
//

#include "IterativeTablesEngine.h"

std::map<char, std::vector<int>> pstMG, pstEG;
std::unordered_map<int, std::string> revparsetable;
std::unordered_map<int, std::string> revparsetableT;

std::string mv1, mv2;
sqlite3* db;
char *zErrMsg;

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


std::set<char> upper {'P', 'R', 'N', 'B', 'Q', 'K'};
std::set<char> lower {'p', 'r', 'n', 'b', 'q', 'k'};

std::map<char, int> piece {
        std::make_pair ('P', 100),
        std::make_pair ('N', 200),
        std::make_pair ('B', 320),
        std::make_pair ('R', 479),
        std::make_pair ('Q', 929),
        std::make_pair ('K', 60000)
};

int A1 = 91;
int H1 = 98;
int A8 = 21;
int H8 = 28;

int N = -10;
int E = 1;
int S = 10;
int W = -1;

std::map<char, std::vector<int>> pstTMG {
        {'P', std::vector<int> (mg_pawn_table, std::end(mg_pawn_table))},
        {'N', std::vector<int> (mg_knight_table, std::end(mg_knight_table))},
        {'B', std::vector<int> (mg_bishop_table, std::end(mg_bishop_table))},
        {'R', std::vector<int> (mg_rook_table, std::end(mg_rook_table))},
        {'Q', std::vector<int> (mg_queen_table, std::end(mg_queen_table))},
        {'K', std::vector<int> (mg_king_table, std::end(mg_king_table))},
};

std::map<char, std::vector<int>> pstTEG {
        {'P', std::vector<int> (eg_pawn_table, std::end(eg_pawn_table))},
        {'N', std::vector<int> (eg_knight_table, std::end(eg_knight_table))},
        {'B', std::vector<int> (eg_bishop_table, std::end(eg_bishop_table))},
        {'R', std::vector<int> (eg_rook_table, std::end(eg_rook_table))},
        {'Q', std::vector<int> (eg_queen_table, std::end(eg_queen_table))},
        {'K', std::vector<int> (eg_king_table, std::end(eg_king_table))},
};

std::map<char, std::vector<int>> directions {
        std::make_pair ('P', std::vector<int> {N, N + N, N + W, N + E}),
        std::make_pair ('N', std::vector<int> {N + N + E, E + N + E, E + S + E, S + S + E, S + S + W,
                                               W + S + W, W + N + W, N + N + W}),
        std::make_pair ('B', std::vector<int> {N + E, S + E, S + W, N + W}),
        std::make_pair ('R', std::vector<int> {N, E, S, W}),
        std::make_pair ('Q', std::vector<int> {N, E, S, W, N + E, S + E, S + W, N + W}),
        std::make_pair ('K', std::vector<int> {N, E, S, W, N + E, S + E, S + W, N + W})
};

int MATE_LOWER = piece['K'] - 10 * piece['Q'];
int MATE_UPPER = piece['K'] + 10 * piece['Q'];

int TABLE_SIZE = 1e7;
int QS_LIMIT = 219;
int EVAL_ROUGHNESS = 13;
bool DRAW_TEST = true;

std::string initial = std::string(
        "         \n"  //   0 -  9
        "         \n"  //  10 - 19
        " rnbqkbnr\n"  //  20 - 29
        " pppppppp\n"  //  30 - 39
        " ........\n"  //  40 - 49
        " ........\n"  //  50 - 59
        " ........\n"  //  60 - 69
        " ........\n"  //  70 - 79
        " PPPPPPPP\n"  //  80 - 89
        " RNBQKBNR\n"  //  90 - 99
        "         \n"  // 100 -109
        "         \n"  // 110 -119
);

bool IterativeTablesEngine::operator== (const IterativeTablesEngine& other) const {
    return this->board == other.board && this->score == other.score && this->whiteCastle == other.whiteCastle &&
           this->blackCastle == other.blackCastle && this->ep == other.ep && this->kp == other.kp;
}

bool IterativeTablesEngine::operator< (const IterativeTablesEngine& other) const {
    if (this->board < other.board) {
        return true;
    } else if (this->board > other.board) {
        return false;
    }
    if (this->score < other.score) {
        return true;
    } else if (this->score > other.score) {
        return false;
    }
    if (this->whiteCastle < other.whiteCastle) {
        return true;
    } else if (this->whiteCastle > other.whiteCastle) {
        return false;
    }
    if (this->blackCastle < other.blackCastle) {
        return true;
    } else if (this->blackCastle > other.blackCastle) {
        return false;
    }
    if (this->ep < other.ep) {
        return true;
    } else if (this->ep > other.ep) {
        return false;
    }
    if (this->kp < other.kp) {
        return true;
    }
    return false;
}

IterativeTablesEngine::IterativeTablesEngine() = default;

IterativeTablesEngine::IterativeTablesEngine(std::string b, int sc, std::pair<bool, bool> wc,
                                             std::pair<bool, bool> bc,
                                             int ep, int kp, std::unordered_map<std::string, int> h,
                                             double ovsc, int f, bool aiblack) {
    this->board = std::move(b);
    this->score = sc;
    this->whiteCastle = wc;
    this->blackCastle = bc;
    this->ep = ep;
    this->kp = kp;
    this->hist = h;
    this->overallScore = ovsc;
    this->figures = f;
    this->AIBlack = aiblack;
};

[[nodiscard]] std::vector<std::pair<std::pair<int, int>, int>> IterativeTablesEngine::genMoves() const {
    std::vector<std::pair<std::pair<int, int>, int>> moves;

    int ActiveBonus = 0;
    for (int i = 0; i < 120; ++i) {
        char p = this->board[i];

        int activeFreeSquares = 0;

        if (!upper.contains(p)) {
            continue;
        }
        std::pair<int, int> turn;
        for (auto d: directions[p]) {
            for (int j = i + d; ; j += d) {
                char q = this->board[j];
                if (q == ' ' || q == '\n' || upper.contains(q)) {
                    break;
                }
                if (p == 'P' && (d == N || d == N + N) && q != '.') {
                    break;
                }
                if (AIBlack) {
                    if (p == 'P' && d == N + N && (i < A1 + N || this->board[i + N] != '.')) {
                        break;
                    }
                } else {
                    if (p == 'P' && d == N + N && (i < H8 + N || this->board[i + N] != '.')) {
                        break;
                    }
                }
                if (p == 'P' && (d == N + W || d == N + E) && q == '.' && j != this->ep && j != this->kp &&
                    j != this->kp - 1 && j != this->kp + 1) {
                    break;
                }
                activeFreeSquares++;
                turn = {i, j};
                moves.emplace_back(turn, value(turn));

                if (p == 'P' || p == 'N' || p == 'K' || lower.contains(q)) {
                    break;
                }
                if (i == A1 && this->board[j + E] == 'K' && this->whiteCastle.first) {
                    activeFreeSquares++;
                    turn = {j + E, j + W};
                    moves.emplace_back(turn, value(turn));
                }
                if (i == H1 && this->board[j + W] == 'K' && this->whiteCastle.second) {
                    activeFreeSquares++;
                    turn = {j + W, j + E};
                    moves.emplace_back(turn, value(turn));
                }
            }
        }
        if (p == 'N') {
            if (overallScore > 30) {
                ActiveBonus += mobility[0][activeFreeSquares].first;
            } else {
                ActiveBonus += mobility[0][activeFreeSquares].second;
            }
        } else if (p == 'B') {
            if (overallScore > 30) {
                ActiveBonus += mobility[1][activeFreeSquares].first;
            } else {
                ActiveBonus += mobility[1][activeFreeSquares].second;
            }
        } else if (p == 'R') {
            if (overallScore > 30) {
                ActiveBonus += mobility[2][activeFreeSquares].first;
            } else {
                ActiveBonus += mobility[2][activeFreeSquares].second;
            }
        } else if (p == 'Q') {
            if (overallScore > 30) {
                ActiveBonus += mobility[3][activeFreeSquares].first;
            } else {
                ActiveBonus += mobility[3][activeFreeSquares].second;
            }
        }
    }

    for (auto & move : moves) {
        move.second += ActiveBonus;
    }
    return moves;
}

[[nodiscard]] IterativeTablesEngine IterativeTablesEngine::rotate() const {
    std::string s = board;
    for (int i = 0; i < 120; ++i) {
        if (upper.contains(s[i])) {
            s[i] += 32;
        } else if (lower.contains(s[i])) {
            s[i] -= 32;
        }
    }
    std::string temp = std::string(s.rbegin(), s.rend());
    IterativeTablesEngine eng(temp, -score, blackCastle, whiteCastle, ep == 0? 0: 119 - ep,
                              kp == 0? 0: 119 - kp, this->hist, overallScore, figures, AIBlack);
    return eng;
}

[[nodiscard]] IterativeTablesEngine IterativeTablesEngine::nullMove() const {
    std::string s = board;
    for (int i = 0; i < 120; ++i) {
        if (upper.find(s[i]) != upper.end()) {
            s[i] += 32;
        } else if (lower.contains(s[i])) {
            s[i] -= 32;
        }
    }
    std::string temp = std::string(s.rbegin(), s.rend());
    IterativeTablesEngine eng(temp, -score, blackCastle, whiteCastle, 0, 0, this->hist,
                              overallScore, figures, AIBlack);
    return eng;
}

IterativeTablesEngine IterativeTablesEngine::move(std::pair<int, int> move) const {
    int i = move.first;
    int j = move.second;
    char p = board[i];
    char q = board[j];

    int figuresCopy = figures;
    double overallScoreCopy = overallScore;

    if (q == 'Q' || q == 'q') {
        figuresCopy--;
        overallScoreCopy -= 9;
    }
    if (q == 'R' || q == 'r') {
        figuresCopy--;
        overallScoreCopy -= 4.5;
    }
    if (q == 'B' || q == 'b') {
        figuresCopy--;
        overallScoreCopy -= 3;
    }
    if (q == 'N' || q == 'n') {
        figuresCopy--;
        overallScoreCopy -= 3;
    }
    if (q == 'P' || q == 'p') {
        overallScoreCopy -= 1;
    }

    std::string boardCopyT = board;
    std::string boardCopy = boardCopyT;
    auto wc = whiteCastle;
    auto bc = blackCastle;
    int epCopy = 0;
    int kpCopy = 0;
    double scoreCopy = score + value(move);

    boardCopy[j] = boardCopy[i];
    boardCopy[i] = '.';

    if (i == A1) {
        wc = {false, wc.second};
    }
    if (i == H1) {
        wc = {wc.first, false};
    }
    if (j == A8) {
        bc = {bc.first, false};
    }
    if (j == H8) {
        bc = {false, bc.second};
    }

    if (p == 'K' || p == 'k') {
        if (p == 'K') {
            wc = {false, false};
        } else {
            bc = {false, false};
        }
        if (std::abs(j - i) == 2) {
            kpCopy = (i + j) / 2;
            if (p == 'K') {
                boardCopy[j < i? A1: H1] = '.';
            } else {
                boardCopy[i < j? A1: H1] = '.';
            }
            boardCopy[kpCopy] = (p == 'K'? 'R': 'r');
        }
    }
    if (p == 'P') {
        if (A8 <= j && j <= H8) {
            boardCopy[j] = (p == 'P'? 'R': 'q');
        }
        if (j - i == 2 * N) {
            epCopy = i + N;
        }
        if (j == this->ep) {
            boardCopy[j + S] = '.';
        }
    }

    return IterativeTablesEngine(boardCopy, scoreCopy, wc, bc, epCopy, kpCopy, this->hist,
                                 overallScoreCopy, figuresCopy, AIBlack).rotate();
}

inline int IterativeTablesEngine::getTable(char p, int ind, double rate) {
    if (lower.contains(p)) {
        p -= 32;
    }
    if (rate > 30) {
        return pstMG[p][ind];
    } else {
        return pstEG[p][ind];
    }
}

int IterativeTablesEngine::value(std::pair<int, int> move) const {
    int i = move.first;
    int j = move.second;
    char p = board[i];
    char q = board[j];

    int scoreCopy = getTable(p, j, overallScore) - getTable(p, i, overallScore);

    if (lower.contains(q)) {
        scoreCopy += getTable(q - 32, 119-j, overallScore);
    }

    if (std::abs(j - this->kp) < 2) {
        scoreCopy += getTable('K', 119 - j, overallScore);
    }

    if (p == 'K' && std::abs(i - j) == 2) {
        scoreCopy += getTable('R', (i + j) / 2, overallScore);
        scoreCopy -= getTable('R', j < i? A1: H1, overallScore);
    }

    if (p == 'P') {
        if (A8 <= j && j <= H8) {
            scoreCopy += getTable('Q', j, overallScore) - getTable('P', j, overallScore);
        }
        if (j == this->ep) {
            scoreCopy += getTable('P', 119 - (j + S), overallScore);
        }
    }
    return scoreCopy;
}


less_than_key::less_than_key(IterativeTablesEngine e) {
    this->e = std::move(e);
}

inline bool less_than_key::operator() (const std::pair<std::pair<int, int>, int>& struct1,
                                       const std::pair<std::pair<int, int>, int>& struct2) {
    return struct1.second > struct2.second;
}

bool Searcher::is_dead(const IterativeTablesEngine& pos) {
    for (auto m: pos.genMoves()) {
        int value = m.second;
        if (value >= MATE_LOWER) {
            return true;
        }
    }
    return false;
}

int Searcher::bound(IterativeTablesEngine pos, double gamma, int depth, bool root) {
    this->nodes++;
    depth = depth > 0? depth: 0;

    if (pos.score <= -MATE_LOWER) {
        return -MATE_UPPER;
    }

    if (DRAW_TEST) {
        if (!root) {
            if (history.contains(pos)) {
                return 0;
            }
        }
    }

    std::pair<int, int> entry;
    auto t = std::make_tuple(pos, depth, root);
    if (!tp_score.contains(t)) {
        entry = {-MATE_UPPER, MATE_UPPER};
    } else {
        entry = tp_score.at(t);
    }

    if (entry.first >= gamma && (!root || tp_move.contains(pos))) {
        return entry.first;
    }
    if (entry.second < gamma) {
        return entry.second;
    }


    // for move, score in moves()
    int best = -MATE_UPPER;
    bool rbnq = false;
    for (auto i = 0; i < 120; ++i) {
        if (pos.board[i] == 'R' || pos.board[i] == 'B' || pos.board[i] == 'N' || pos.board[i] == 'Q') {
            rbnq = true;
            break;
        }
    }

    std::vector<std::pair<std::pair<int, int>, int>> moves;
    if (depth > 0 && !root && rbnq) {
        auto res = -bound(pos.nullMove(), 1 - gamma,
                          depth - 3, false);
        best = best > res? best: res;

        if (res >= gamma && pos.score > -MATE_LOWER) {
            if (tp_move.size() > TABLE_SIZE) {
                tp_move.clear();
            }
            tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
            return res;
        }

        moves.push_back({{-1, -1}, res});
    }
    if (depth == 0) {
        auto res = pos.score;
        best = best > res? best: res;

        if (res >= gamma) {
            if (tp_move.size() > TABLE_SIZE) {
                tp_move.clear();
            }
            tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
            return res;
        }

        moves.push_back({{-1, -1}, res});
    }

    if (tp_move.contains(pos)) {
        auto killer = tp_move.at(pos);
        if (depth > 0 || pos.value(killer) >= QS_LIMIT) {
            auto res = -bound(pos.move(killer), 1 - gamma, depth - 1, false);
            if (res >= gamma) {
                if (tp_move.size() > TABLE_SIZE) {
                    tp_move.clear();
                }
                if (killer.first != -1 && killer.second != -1) {
                    tp_move[pos] = killer;
                }
                tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
                return res;
            }
            best = best > res? best: res;
            moves.emplace_back(killer, res);
        }
    }

    auto x = pos.genMoves();
    auto komp = less_than_key(pos);
    std::sort(x.begin(), x.end(), komp);

    for (auto move: x) {
        int value = move.second;
        if (depth > 0 || value >= QS_LIMIT) {
            auto res = -bound(pos.move(move.first), 1 - gamma, depth - 1, false);
            if (res >= gamma) {
                if (tp_move.size() > TABLE_SIZE) {
                    tp_move.clear();
                }
                if (move.first.first != -1 && move.first.second != -1) {
                    tp_move[pos] = move.first;
                }
                tp_score[std::make_tuple(pos, depth, root)] = {res, entry.second};
                return res;
            }
            best = best > res? best: res;
            moves.emplace_back(move.first, res);
        }
    }

    if (best < gamma && best < 0 && depth > 0) {
        bool ok = true;
        for (auto m: x) {
            if (!is_dead(pos.move(m.first))) {
                ok = false;
                break;
            }
        }
        if (ok) {
            best = is_dead(pos.nullMove())? -MATE_UPPER: 0;
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

std::tuple<int, std::pair<int, int>, int> Searcher::search(const IterativeTablesEngine& pos,
                                                           const std::vector<IterativeTablesEngine>& h,
                                                           int time_control) {
    nodes = 0;
    if (DRAW_TEST) {
        this->history = std::set<IterativeTablesEngine>();
        for (const auto& x : h) {
            this->history.insert(x);
        }
        this->tp_score.clear();
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int depth = 1; depth < 50; ++depth) {
        std::cout << depth << std::endl;
        int lw = -MATE_UPPER;
        int up = MATE_UPPER;
        while (lw < up - EVAL_ROUGHNESS) {
            int gamma = (lw + up + 1) / 2;
            int score = bound(pos, gamma, depth);
//                std::cout << lw << ' ' << up << ' ' << gamma << ' ' << score << std::endl;
            if (score >= gamma) {
                lw = score;
            }
            if (score < gamma) {
                up = score;
            }
        }
        bound(pos, lw, depth);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() >= time_control) {
            return std::make_tuple(depth, tp_move[pos], tp_score[std::make_tuple(pos, depth, true)].first);
        }
    }
}

void printPos(IterativeTablesEngine pos) {
    for (int i = 0; i < 120; ++i) {
        if (i < 20 || i >= 100) {
            continue;
        }
        std::cout << pos.board[i];
    }
    std::cout << std::endl;
}

int parse(std::string s, bool AIBlack, bool mode, bool aiTurn) {
    int upper = 91;
    if (aiTurn) {
        upper = (AIBlack? 91: 98);
    }
    if (mode) {
        int fil = s[0] - 'a';
        int rank = s[1] - '0' - 1;
        return upper + fil - 10 * rank;
    } else {
        int fil = s[0] - 'a';
        fil = 7 - fil;
        int rank = s[1] - '0' - 1;
        rank = 7 - rank;
        return upper + fil - 10 * rank;
    }
}

std::string backparse(int x, bool AIMove) {
    if (!AIMove) {
        std::string r = revparsetable[x];
        return std::string(1, 'a' + 'h' - r[0]) + std::string(1, '1' + '8' - r[1]);
    } else {
        std::string r = revparsetableT[x];
        return std::string(1, 'a' + 'h' - r[0]) + std::string(1, '1' + '8' - r[1]);
    }
}

std::string reverse(const std::string& s) {
    if (s.size() == 4) {
        return std::string(1, 'a' + 'h' - s[0]) +
               std::string(1, '1' + '8' - s[1]) +
               std::string(1, 'a' + 'h' - s[2]) +
               std::string(1, '1' + '8' - s[3]);
    } else if (s.size() == 2) {
        return std::string(1, 'a' + 'h' - s[1]) +
               std::string(1, '1' + '8' - s[1]);
    }
}
/*
void go () {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::string x = std::string(1, 'a' + i) + std::to_string(j + 1);
            revparsetable[parse(x, true)] = x;  // sec - AIBlack
        }
    }
    for (auto const& pair: pstTMG) {
        std::vector<int> x;
        for (int j = 0; j < 20; ++j) {
            x.push_back(0);
        }
        x.push_back(0);
        for (int j = 0; j < pair.second.size(); ++j) {
            x.push_back(pair.second[j] + piece[pair.first]);
            if (j % 8 == 7) {
                x.push_back(0);
                x.push_back(0);
            }
        }
        for (int j = 0; j < 19; ++j) {
            x.push_back(0);
        }
        pstMG[pair.first] = x;
    }

    for (auto const& pair: pstTEG) {
        std::vector<int> x;
        for (int j = 0; j < 20; ++j) {
            x.push_back(0);
        }
        x.push_back(0);
        for (int j = 0; j < pair.second.size(); ++j) {
            x.push_back(pair.second[j] + piece[pair.first]);
            if (j % 8 == 7) {
                x.push_back(0);
                x.push_back(0);
            }
        }
        for (int j = 0; j < 19; ++j) {
            x.push_back(0);
        }
        pstEG[pair.first] = x;
    }
    std::vector<IterativeTablesEngine> hist {IterativeTablesEngine(initial, 0, {true, true},
                                                                   {true, true}, 0, 0,
                                                                   std::unordered_map<std::string, int> (),
                                                                   76.0, 16,
                                                                   true)};  // lst - isAI black
    Searcher searcher;
    while (true) {
        printPos(hist[hist.size() - 1]);
        if (hist[hist.size() - 1].score <= -MATE_LOWER) {
            std::cout << "You lost" << std::endl;
            break;
        }

        std::string move;
        std::cin >> move;
        std::string f = move.substr(0, 2);
        std::string s = move.substr(2, 2);
        hist.push_back(hist[hist.size() - 1].move({parse(f, true),
                                                   parse(s, true)}));  // sec - AIBlack

        printPos(hist[hist.size() - 1].rotate());

        if (hist[hist.size() - 1].score <= -MATE_LOWER) {
            std::cout << "You won" << std::endl;
            break;
        }

        auto res = searcher.search(hist[hist.size() - 1], hist);
        int depth = std::get<0> (res);
        std::pair<int, int> mv = std::get<1> (res);
        int score = std::get<2> (res);

        std::cout << backparse(mv.first) << ' ' << backparse(mv.second) << ' ' << score / 10.0 << ' ' <<
                  depth << std::endl;

        if (score == MATE_UPPER) {
            std::cout << "Checkmate!" << std::endl;
        }
        hist.push_back(hist[hist.size() - 1].move(mv));
    }
}*/

void fillTables(bool aiblack) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::string x = std::string(1, 'a' + i) + std::to_string(j + 1);
            std::cout << x << ' ' << parse(x, aiblack) << std::endl;
            revparsetable[parse(x, aiblack)] = x;
            revparsetableT[parse(x, aiblack, false)] = x;
        }
    }
    for (auto const& pair: pstTMG) {
        std::vector<int> x;
        for (int j = 0; j < 20; ++j) {
            x.push_back(0);
        }
        x.push_back(0);
        for (int j = 0; j < pair.second.size(); ++j) {
            x.push_back(pair.second[j] + piece[pair.first]);
            if (j % 8 == 7) {
                x.push_back(0);
                x.push_back(0);
            }
        }
        for (int j = 0; j < 19; ++j) {
            x.push_back(0);
        }
        pstMG[pair.first] = x;
    }

    for (auto const& pair: pstTEG) {
        std::vector<int> x;
        for (int j = 0; j < 20; ++j) {
            x.push_back(0);
        }
        x.push_back(0);
        for (int j = 0; j < pair.second.size(); ++j) {
            x.push_back(pair.second[j] + piece[pair.first]);
            if (j % 8 == 7) {
                x.push_back(0);
                x.push_back(0);
            }
        }
        for (int j = 0; j < 19; ++j) {
            x.push_back(0);
        }
        pstEG[pair.first] = x;
    }
}
/*
void play () {
    bool AIplaysBlack = false;

    fillTables(AIplaysBlack);

//    init();
//    zErrMsg = nullptr;
//    sqlite3_open(getMove"test1.db", &db);

    auto* rules = new thc::ChessRules;
    rules->Init();
    thc::Move move = thc::Move();

    Searcher searcher;
    std::vector<IterativeTablesEngine> hist {IterativeTablesEngine(initial, 0, {true, true},
                                                                   {true, true}, 0, 0,
                                                                   std::unordered_map<std::string, int> (),
                                                                   76.0, 16, AIplaysBlack)};

    std::string mv, sM;
    thc::DRAWTYPE d;
    std::pair<int, int> mv_;

    if (!AIplaysBlack) {
        H8 = 91;
        A1 = 28;
        A8 = 98;
        H1 = 21;
    }

    int dd = 0;

    printPos(hist[hist.size() - 1]);
    while (true) {
        dd++;
        if (AIplaysBlack) {
            std::string str;
            std::cin >> str;
            move.NaturalIn(rules, str.c_str());
            rules->PlayMove(move);
            sM = move.TerseOut();
            hist.push_back(hist[hist.size() - 1].move({parse(sM.substr(0, 2), AIplaysBlack),
                                                       parse(sM.substr(2, 2), AIplaysBlack)}));

            printPos(hist[hist.size() - 1].rotate());
            std::cout << "Repetition: " << rules->GetRepetitionCount() << std::endl;
            if (hist[hist.size() - 1].score <= -MATE_LOWER) {
                std::cout << "You won" << std::endl;
                break;
            }
            if (rules->IsDraw(true, d)) {
                std::cout << "draw" << ' ' << d << std::endl;
                break;
            }

            mv = "";//getMove(rules, hist[hist.size() - 1]);
            if (!mv.empty()) {
                move.NaturalInFast(rules, mv.c_str());
                sM = move.TerseOut();
                std::cout << sM << std::endl;
                rules->PlayMove(move);

                hist.push_back(hist[hist.size() - 1].move({parse(sM.substr(0, 2),
                                                                 AIplaysBlack,
                                                                 false),
                                                           parse(sM.substr(2, 2), AIplaysBlack,
                                                                 false)}));
                printPos(hist[hist.size() - 1]);

                if (hist[hist.size() - 1].score == MATE_UPPER) {
                    std::cout << "Checkmate!" << std::endl;
                    break;
                }
            } else {
                auto res = searcher.search(hist[hist.size() - 1], hist);
                int depth = std::get<0> (res);
                mv_ = std::get<1> (res);
                int score = std::get<2> (res);

                std::string beg, end;
                beg = backparse(mv_.first);
                end = backparse(mv_.second);
                move.NaturalIn(rules, reverse(beg + end).c_str());
                std::cout << beg << ' ' << end << ' ' << score / 10.0 << ' ' << depth << std::endl;
                rules->PlayMove(move);
                hist.push_back(hist[hist.size() - 1].move(mv_));
                printPos(hist[hist.size() - 1]);
                if (score == MATE_UPPER) {
                    std::cout << "Checkmate!" << std::endl;
                    break;
                }
            }
            if (rules->IsDraw(true, d)) {
                std::cout << "draw" << ' ' << d << std::endl;
                break;
            }
        } else {
            mv = "";//getMove(rules, hist[hist.size() - 1]);
            if (!mv.empty()) {
                if (dd == 1) {
                    hist[hist.size() - 1] = hist[hist.size() - 1].rotate();
                }
                std::cout << mv << std::endl;
//                mv = reverse(mv);
                move.NaturalInFast(rules, mv.c_str());  // todo reverse?
                sM = move.TerseOut();
                std::cout << sM << std::endl;
                rules->PlayMove(move);

                hist.push_back(hist[hist.size() - 1].move({parse(sM.substr(0, 2), AIplaysBlack,
                                                                 false),
                                                           parse(sM.substr(2, 2), AIplaysBlack,
                                                                 false)}));
                printPos(hist[hist.size() - 1]);

                if (hist[hist.size() - 1].score == MATE_UPPER) {
                    std::cout << "Checkmate!" << std::endl;
                    break;
                }
            } else {
                auto res = searcher.search(hist[hist.size() - 1].rotate(), hist);
                int depth = std::get<0> (res);
                mv_ = std::get<1> (res);
                int score = std::get<2> (res);

                std::string beg, end;
                beg = backparse(mv_.first);
                end = backparse(mv_.second);
                move.NaturalIn(rules, reverse(beg + end).c_str());
                std::cout << move.TerseOut() << ' ' << score / 10.0 << ' ' << depth << std::endl;
                rules->PlayMove(move);
                mv_ = {parse(beg, AIplaysBlack), parse(end, AIplaysBlack)};
                hist.push_back(hist[hist.size() - 1].move(mv_));
                printPos(hist[hist.size() - 1]);
                if (score == MATE_UPPER) {
                    std::cout << "Checkmate!" << std::endl;
                    break;
                }
            }
            if (rules->IsDraw(true, d)) {
                std::cout << "draw" << ' ' << d << std::endl;
                break;
            }

            std::string str;
            std::cin >> str;
            move.NaturalIn(rules, str.c_str());
            rules->PlayMove(move);
            sM = move.TerseOut();
            hist.push_back(hist[hist.size() - 1].move({parse(str.substr(0, 2),
                                                             AIplaysBlack),
                                                       parse(str.substr(2, 2),
                                                             AIplaysBlack)}));

            printPos(hist[hist.size() - 1].rotate());
            std::cout << "Repetition: " << rules->GetRepetitionCount() << std::endl;
            if (hist[hist.size() - 1].score <= -MATE_LOWER) {
                std::cout << "You won" << std::endl;
                break;
            }
            if (rules->IsDraw(true, d)) {
                std::cout << "draw" << ' ' << d << std::endl;
                break;
            }
        }

    }

    sqlite3_close(db);
}*/

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    mv1 = argv[1];
    if (argv[2] == nullptr) {
        mv2 = "NULL";
    } else {
        mv2 = argv[2];
    }
    return 0;
}
static int callbackEx(void *NotUsed, int argc, char **argv, char **azColName) {
    if (std::stoi(argv[1]) > 0) {
        std::string request = "SELECT * FROM MOVES WHERE ID = '" + std::string(argv[0]) + "'";
        sqlite3_exec(db, request.c_str(), callback, nullptr, &zErrMsg);
    } else {
        mv1 = "NULL";
    }
    return 0;
}

std::string endingDB(const std::string& pos) {
    return search(pos);
}

std::string getMove(thc::ChessRules* rules, const IterativeTablesEngine& eng, bool op, bool end) {
    int figs = eng.figures;
    std::string pos = rules->ForsythPublish();

    if (end) {
        if (figs <= 5) {
            std::string edb = endingDB(pos);
            if (!edb.empty()) {
                return edb;
            }
        }
    }

    if (op) {
        std::string odb = openingDB(pos);
        if (!odb.empty()) {
            return odb;
        }
    }

    return "";
}

std::string openingDB(const std::string& pos) {
    std::string request = "SELECT ID, COUNT(ID) FROM MOVES WHERE ID = '" + pos + "'";
    mv1 = mv2 = "";
    sqlite3_exec(db, request.c_str(), callbackEx, nullptr, &zErrMsg);
    if (mv1 == "NULL") {
        return "";
    } else if (mv2 == "NULL") {
        return mv1;
    }
    int choice = rand(0, 1) % 2;
    if (choice) {
        return mv1;
    } else {
        return mv2;
    }
}

