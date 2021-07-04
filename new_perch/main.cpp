//#pragma GCC optimization ("unroll-loops")
//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx,avx2,fma")

#include "Engine.h"

Move makeMove(const Position& pos, int begin, int end) {
    Move mv(pos.board[begin], begin, end, pos.board[end] != ' ');
    if (pos.board[begin] == 'p') {
        mv.piece = 1;
    } else if (pos.board[begin] == 'r') {
        mv.piece = 2;
    } else if (pos.board[begin] == 'n') {
        mv.piece = 3;
    } else if (pos.board[begin] == 'b') {
        mv.piece = 4;
    } else if (pos.board[begin] == 'k') {
        mv.piece = 6;
    } else if (pos.board[begin] == 'q') {
        mv.piece = 5;
    }
    if (pos.enpassant == (1ull << end)) {
        mv.isCapture = true;
        mv.enpassant = true;
    }
    if (mv.piece == 1 && 8 <= begin && begin <= 15) {
        mv.promotion = 1;
    }
    if (mv.piece == 6) {
        if (abs(end - begin) == 2) {
            if (end == 62) {
                mv.isCastleKingSide = true;
            } else {
                mv.isCastleQueenSide = true;
            }
        }
    }
    return mv;
}

int conversion(std::string s) {
    int file = s[0] - 'a';
    int rank = s[1] - '1';
    return 8 * rank + file;
}

std::string backConversion(Move mv) {
    uint64_t rankbegin = mv.src >> 3;
    uint64_t rankend = mv.dst >> 3;
    uint64_t filebegin = mv.src & 7;
    uint64_t fileend = mv.dst & 7;

    std::string s;
    s += 'a' + filebegin;
    s += '1' + rankbegin;
    s += 'a' + fileend;
    s += '1' + rankend;
    return s;
}

int main() {
    Position pos;

    pos.Initialize();

    std::vector<Position> hist{pos};

    Engine engine;

    std::cout << pos.board << std::endl;

    int counter = 0;
//    while (true) {
//        engine.search(pos, hist, 20000);
//        auto move = engine.tp_move[pos];
//        pos = pos.playMoveCopy(move, pos.white2play);
//        std::cout << pos.score << std::endl;
//        hist.push_back(pos);
//        for (int i = 1; i <= 64; ++i) {
//            std::cout << (pos.board[i - 1] != ' '? pos.board[i - 1]: '.');
//            if (i % 8 == 0) {
//                std::cout << std::endl;
//            }
//        }
//
//        std::cout << std::endl;
//    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    std::cout << Perft(5, pos) << std::endl;
//    std::cout << count << std::endl;
//    for (int i = 0; i < 4865609; i += 1) {
//        pos.genMoves();
//        pos.playMoveCopy(pos.moves[0].first, pos.white2play);
//    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;

    while (true) {
        if (counter % 2 == 0) {
            engine.search(pos, hist, 20000);
            auto move = engine.tp_move[pos];
            pos = pos.playMoveCopy(move, pos.white2play);
            std::cout << backConversion(move) << std::endl;
        } else {
            std::string s;
            std::cin >> s;
            pos = pos.playMoveCopy(makeMove(pos, conversion(s.substr(0, 2)),
                                            conversion(s.substr(2, 2))), pos.white2play);

        }
        std::cout << pos.score << std::endl;
        hist.push_back(pos);
        for (int i = 1; i <= 64; ++i) {
            std::cout << (pos.board[i - 1] != ' '? pos.board[i - 1]: '.');
            if (i % 8 == 0) {
                std::cout << std::endl;
            }
        }

        std::cout << std::endl;
        counter++;
    }

//    ABP abp;
//    while (true) {
//        if (counter % 2 == 0) {
//            Move move = abp.getMove(pos);
//            pos = pos.playMoveCopy(move, pos.white2play);
//            std::cout << backConversion(move) << std::endl;
//        } else {
//            std::string s;
//            std::cin >> s;
//            pos = pos.playMoveCopy(makeMove(pos, conversion(s.substr(0, 2)),
//                                            conversion(s.substr(2, 2))), pos.white2play);
//
//        }
//        std::cout << pos.score << std::endl;
//        hist.push_back(pos);
//        for (int i = 1; i <= 64; ++i) {
//            std::cout << (pos.board[i - 1] != ' '? pos.board[i - 1]: '.');
//            if (i % 8 == 0) {
//                std::cout << std::endl;
//            }
//        }
//
//        std::cout << std::endl;
//        counter++;
//    }
    return 0;
}
