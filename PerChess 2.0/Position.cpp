//
// Created by afentev on 5/30/21.
//

#include "Position.h"

inline __attribute__((always_inline)) Bitboard Position::eastOne (Bitboard b) noexcept {return (b << 1) & notAFile;}
inline __attribute__((always_inline)) Bitboard Position::noEaOne (Bitboard b) noexcept {return (b << 9) & notAFile;}
inline __attribute__((always_inline)) Bitboard Position::soEaOne (Bitboard b) noexcept {return (b >> 7) & notAFile;}
inline __attribute__((always_inline)) Bitboard Position::westOne (Bitboard b) noexcept {return (b >> 1) & notHFile;}
inline __attribute__((always_inline)) Bitboard Position::soWeOne (Bitboard b) noexcept {return (b >> 9) & notHFile;}
inline __attribute__((always_inline)) Bitboard Position::noWeOne (Bitboard b) noexcept {return (b << 7) & notHFile;}

inline __attribute__((always_inline)) Bitboard Position::wPawnEastAttacks(Bitboard wpawns) noexcept {return noEaOne(wpawns);}
inline __attribute__((always_inline)) Bitboard Position::wPawnWestAttacks(Bitboard wpawns) noexcept {return noWeOne(wpawns);}
inline __attribute__((always_inline)) Bitboard Position::wPawnAllAttacks(Bitboard wpawns) noexcept {
    return wPawnEastAttacks(wpawns) | wPawnWestAttacks(wpawns);
}

inline __attribute__((always_inline)) Bitboard Position::bPawnEastAttacks(Bitboard bpawns) noexcept {return soEaOne(bpawns);}
inline __attribute__((always_inline)) Bitboard Position::bPawnWestAttacks(Bitboard bpawns) noexcept {return soWeOne(bpawns);}
inline __attribute__((always_inline)) Bitboard Position::bPawnAllAttacks(Bitboard bpawns) noexcept {
    return bPawnEastAttacks(bpawns) | bPawnWestAttacks(bpawns);
}

inline __attribute__((always_inline)) void Position::pawnNonCaptures(Bitboard pawns, Bitboard enemies, Bitboard friendly,
                               bool side2play, std::vector<Move>& moves) noexcept {
    uint64_t appendix, doubleAppendix;
    Bitboard promotion;

    enemies |= friendly;
    Bitboard free = ~enemies;
    Bitboard pushes, doublePushes;
    if (side2play) {
        appendix = 8;
        doubleAppendix = 16;
        pushes = (pawns << 8) & free;
        doublePushes = ((pushes & isThirdRank) << 8) & free;
        promotion = isLastRank;
    } else {
        appendix = -8;
        doubleAppendix = -16;
        pushes = (pawns >> 8) & free;
        doublePushes = ((pushes & isThirdRankBack) >> 8) & free;
        promotion = isFirstRank;
    }

    while (pushes) {
        uint64_t index = __builtin_ctzll(pushes);
        uint64_t x = index - appendix;
//        moves.emplace_back(1, x, index, false, (promotion & (1ull << index)) != 1);
        if (promotion & (1ull << index)) {
            moves.emplace_back(1, x, index, false, 1);
            moves.emplace_back(1, x, index, false, 2);
            moves.emplace_back(1, x, index, false, 3);
            moves.emplace_back(1, x, index, false, 4);
        } else {
            moves.emplace_back(1, x, index, false);
        }
        pushes &= pushes - 1;
    }

    while (doublePushes) {
        uint64_t index = __builtin_ctzll(doublePushes);
        moves.emplace_back(1, index - doubleAppendix, index, false);
        doublePushes &= doublePushes - 1;
    }
}

inline __attribute__((always_inline)) void Position::pawnCaptures(Bitboard pawns, Bitboard enemies, Bitboard enpas,
                            bool side2play, std::vector<Move>& moves) noexcept {
    int arrIndex;
    Bitboard promotion;
    if (side2play) {
        arrIndex = 0;
        promotion = isLastRank;
    } else {
        arrIndex = 1;
        promotion = isFirstRank;
    }
    enemies |= enpas;
    while (pawns) {
        uint64_t index = __builtin_ctzll(pawns);
        Bitboard res = arrPawnAttacks[arrIndex][index] & enemies;
        bool prom = res & promotion;
        if (prom) {
            while (res) {
                uint64_t target = __builtin_ctzll(res);
                moves.emplace_back(1, index, target, true, 1);
                moves.emplace_back(1, index, target, true, 2);
                moves.emplace_back(1, index, target, true, 3);
                moves.emplace_back(1, index, target, true, 4);
                res &= res - 1;
            }
        } else {
            while (res) {
                uint64_t target = __builtin_ctzll(res);
                Move mv(1, index, target, true);
                Bitboard tmp = 1ull << target;
                if (enpas == tmp) {
                    mv.enpassant = true;
                }
                moves.push_back(mv);
                res &= res - 1;
            }
        }
        pawns &= pawns - 1;
    }
}

inline __attribute__((always_inline)) void Position::pawnMoves(Bitboard pawns, Bitboard enemies, Bitboard friendly,
                         Bitboard enpas, bool side2play, std::vector<Move>& moves) noexcept {
    pawnNonCaptures(pawns, enemies, friendly, side2play, moves);
    pawnCaptures(pawns, enemies, enpas, side2play, moves);
}

inline __attribute__((always_inline)) int Position::rookMoves(Bitboard rooks, Bitboard enemies, Bitboard friendly, bool isEndgame,
                        std::vector<Move>& moves, int pieceType) noexcept {
    int mobility = 0;

    while (rooks) {
        uint64_t index = __builtin_ctzll(rooks);
        Bitboard attacks = slider_attacks.RookAttacks(enemies | friendly,
                                                      static_cast<int>(index)) & ~friendly;

        mobility += mobilityTable[pieceType][__builtin_popcountll(attacks)][isEndgame];

        while (attacks) {
            uint64_t target = __builtin_ctzll(attacks);
            moves.emplace_back(pieceType, index, target, (enemies >> target) & 1);
            attacks &= attacks - 1;
        }

        rooks &= rooks - 1;
    }
    return mobility;
}

inline __attribute__((always_inline)) int Position::bishopMoves(Bitboard bishops, Bitboard enemies, Bitboard friendly, bool isEndgame,
                          std::vector<Move>& moves, int pieceType) noexcept {
    int mobility = 0;
    while (bishops) {
        uint64_t index = __builtin_ctzll(bishops);
        Bitboard attacks = slider_attacks.BishopAttacks(enemies | friendly,
                                                        static_cast<int>(index)) & ~friendly;

        mobility += mobilityTable[pieceType][__builtin_popcountll(attacks)][isEndgame];

        while (attacks) {
            uint64_t target = __builtin_ctzll(attacks);
            moves.emplace_back(pieceType, index, target, (enemies >> target) & 1);
            attacks &= attacks - 1;
        }

        bishops &= bishops - 1;
    }

    return mobility;
}

inline __attribute__((always_inline)) int Position::queenMoves(Bitboard queens, Bitboard enemies, Bitboard friendly,
                         bool isEndgame, std::vector<Move>& moves) noexcept {
    int mobility = 0;
    mobility += rookMoves(queens, enemies, friendly, isEndgame, moves, 5);
    mobility += bishopMoves(queens, enemies, friendly, isEndgame, moves, 5);
    return mobility;
}

inline __attribute__((always_inline)) int Position::knightMoves(Bitboard knights, Bitboard enemies, Bitboard friendly,
                          bool isEndgame, std::vector<Move>& moves) noexcept {
    int mobility = 0;
    while (knights) {
        uint64_t index = __builtin_ctzll(knights);
        Bitboard res = knightPosMoves[index] & ~friendly;

        mobility += mobilityTable[0][__builtin_popcountll(res)][isEndgame];

        while (res) {
            uint64_t target = __builtin_ctzll(res);
            moves.emplace_back(3, index, target, (enemies >> target) & 1);
            res &= res - 1;
        }

        knights &= knights -1 ;
    }
    return mobility;
}

inline __attribute__((always_inline)) void Position::kingMoves(Bitboard kings, Bitboard enemies, Bitboard friendly, Bitboard friendlyRooks,
                         bool qcastle, bool kcastle, bool side2play, std::vector<Move>& moves) noexcept {
    if (kings == 0) {
        return;
    }

    while (kings) {
        uint64_t index = __builtin_ctzll(kings);
        Bitboard res;
        res = kingPosMoves[index] & ~friendly;
        while (res) {
            uint64_t target = __builtin_ctzll(res);
            moves.emplace_back(6, index, target, (enemies >> target) & 1);
            res &= res - 1;
        }
        kings &= kings - 1;
    }

    Bitboard pieces = enemies | friendly;
    if (kcastle) {
        if (side2play) {
            if (!((pieces >> 5ull) & 1) && !((pieces >> 6ull) & 1) && (friendlyRooks >> 7ull) & 1) {
                moves.emplace_back(6, 4ull, 6ull, false, false,
                                   true);
            }
        } else {
            if (!((pieces >> 61ull) & 1) && !((pieces >> 62ull) & 1) && (friendlyRooks >> 63ull) & 1) {
                moves.emplace_back(6, 60ull, 62ull, false, false,
                                   true);
            }
        }
    }

    if (qcastle) {
        if (side2play) {
            if (!((pieces >> 1ull) & 1) && !((pieces >> 2ull) & 1) && !((pieces >> 3ull) & 1) &&
                friendlyRooks & 1) {
                moves.emplace_back(6, 4ull, 2ull, false, true,
                                   false);
            }
        } else {
            if (!((pieces >> 57ull) & 1) && !((pieces >> 58ull) & 1) && !((pieces >> 59ull) & 1) &&
                (friendlyRooks >> 56ull) & 1) {
                moves.emplace_back(6, 60ull, 58ull, false, true,
                                   false);
            }
        }
    }
}

inline __attribute__((always_inline)) int Position::allMoves(Bitboard pawns, Bitboard knights, Bitboard bishops,
                       Bitboard rooks, Bitboard queens, Bitboard kings, Bitboard enpas,
                       Bitboard enemies, Bitboard friendly, bool qcastle, bool kcastle,
                       bool side2play, bool isEndgame, std::vector<Move>& res) noexcept {
    int bonus = 0;

    bonus += bishopMoves(bishops, enemies, friendly, isEndgame, res);
//
    bonus += rookMoves(rooks, enemies, friendly, isEndgame, res);
//
    pawnMoves(pawns, enemies, friendly, enpas, side2play, res);
//
    bonus += knightMoves(knights, enemies, friendly, isEndgame, res);
//
    kingMoves(kings, enemies, friendly, rooks, qcastle, kcastle, side2play, res);
//
    bonus += queenMoves(queens, enemies, friendly, isEndgame, res);

    return bonus;
}

void Position::Initialize() {
    board = "RNBQKBNR"
            "PPPPPPPP"
            "        "
            "        "
            "        "
            "        "
            "pppppppp"
            "rnbqkbnr";


    whitePawns = 0xff00;
    blackPawns = 0xff000000000000;

    whiteKnights = 0x42;
    blackKnights = 0x4200000000000000;

    whiteBishops = 0x24;
    blackBishops = 0x2400000000000000;

    whiteRooks = 0x81;
    blackRooks = 0x8100000000000000;

    whiteQueens = 0x8;
    blackQueens = 0x800000000000000;

    whiteKings = 0x10;
    blackKings = 0x1000000000000000;

    whitePieces = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKings;
    blackPieces = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKings;

    occupied = whitePieces | blackPieces;
    empty = ~occupied;

    enpassant = 0x0;
    white2play = true;

    overallScore =
            (piece['P'] << 4) + (piece['Q'] << 1) + (piece['R'] << 2) + (piece['B'] << 2) + (piece['N'] << 2);

    slider_attacks.Initialize();

    fillTables();

    moves = genMoves();
}

void Position::makeCapture(Move& move, uint64_t destination, uint64_t& appendixForEnemies,
                                  Bitboard& enemiesQueens, Bitboard& enemiesBishops, Bitboard& enemiesKnights,
                                  Bitboard& enemiesRooks, Bitboard& enemiesPawns,
                                  Bitboard& enemiesKings, bool& qcastle, bool& kcastle,
                                  int enpasOffset) noexcept {
    if (board[move.dst] + appendixForEnemies == 'k') {
        qcastle = kcastle = false;
        enemiesKings = 0;  // mate
    } else if (move.enpassant) {
        if (enpasOffset == -8) {
            destination >>= 8;
        } else {
            destination <<= 8;
        }
        enemiesPawns ^= destination;
    } else {
        enemiesPawns &= ~destination;
        enemiesBishops &= ~destination;
        enemiesKnights &= ~destination;
        enemiesRooks &= ~destination;
        enemiesQueens &= ~destination;
    }
}

Position Position::playMoveCopy(Move move, bool side2play) const noexcept {
    Position pos;
    pos.whitePawns = whitePawns;
    pos.whiteBishops = whiteBishops;
    pos.whiteKnights = whiteKnights;
    pos.whiteRooks = whiteRooks;
    pos.whiteQueens = whiteQueens;
    pos.whiteKings = whiteKings;
    pos.blackPawns = blackPawns;
    pos.blackBishops = blackBishops;
    pos.blackKnights = blackKnights;
    pos.blackRooks = blackRooks;
    pos.blackQueens = blackQueens;
    pos.blackKings = blackKings;
    pos.board = board;
    pos.whiteKingSideCastle = whiteKingSideCastle;
    pos.whiteQueenSideCastle = whiteQueenSideCastle;
    pos.blackKingSideCastle = blackKingSideCastle;
    pos.blackQueenSideCastle = blackQueenSideCastle;
    pos.overallScore = overallScore;
    pos.score = score;
    pos.pawns = pawns;
    pos.whitePositional = whitePositional;
    pos.blackPositional = blackPositional;
    pos.whitePositional_tmp = whitePositional_tmp;
    pos.blackPositional_tmp = blackPositional_tmp;

    int moveEval = value(move);
    pos.overallScore -= piece[static_cast<char>(pos.board[move.dst] - !side2play * 32)];
    pos.enpassant = 0x0;
    jumpTable[side2play]
             [move.piece - 1]
             [move.isCapture]
             [(move.promotion != 0) + move.isCastleKingSide + 2 * move.isCastleQueenSide](pos, move);

    pos.whitePieces =
            pos.whitePawns | pos.whiteRooks | pos.whiteKnights | pos.whiteBishops |
            pos.whiteQueens | pos.whiteKings;

    pos.blackPieces =
            pos.blackPawns | pos.blackRooks | pos.blackKnights | pos.blackBishops |
            pos.blackQueens | pos.blackKings;

    pos.occupied = pos.whitePieces | pos.blackPieces;
    pos.empty = ~pos.occupied;

    pos.white2play = !white2play;

    pos.pawns = __builtin_popcountll(pos.whitePawns | pos.blackPawns);
    pos.moves = pos.genMoves();

    Bitboard aOpenFiles = openFiles(pos.whitePawns, pos.blackPawns);

    if (!pos.white2play) {
        Bitboard wHalfOpenFiles = wHalfOpenFile(pos.whitePawns, pos.blackPawns);

        int positional = __builtin_popcountll(pos.whitePawns & center) * 40;  // occupying the focal center
        positional += __builtin_popcountll((pos.whiteBishops | pos.whiteKnights) & center) * 20;  // light pieces
        positional += __builtin_popcountll(pos.whiteQueens & center) * 30;  // queen
        positional += __builtin_popcountll(wPawnAllAttacks(pos.whitePawns) & advancedCenter) * 10;
        positional += __builtin_popcountll(((pos.whitePieces) & (~pos.whiteKings)) & advancedOnlyCenter) * 10;

        Bitboard isQueenOnOFile = aOpenFiles & pos.whiteQueens;
        Bitboard isQueenOnHFile = wHalfOpenFiles & pos.whiteQueens;
        if (isQueenOnOFile) {
            if ((isQueenOnOFile & pos.blackKings) != 0) {
                positional += 20;
            } else {
                positional += 15;
            }
        } else if (isQueenOnHFile) {
            positional += 10;
        }

        Bitboard isRookOnOFile = aOpenFiles & pos.whiteRooks;
        Bitboard isRookOnHFile = wHalfOpenFiles & pos.whiteRooks;
        positional += 15 * __builtin_popcountll(isRookOnHFile);
        positional += 3 * __builtin_popcountll(isRookOnOFile);

        pos.score = -(pos.score + (positional + pos.whitePositional_tmp - whitePositional) + moveEval);
        pos.whitePositional = pos.whitePositional_tmp + positional;
    } else {
        Bitboard bHalfOpenFiles = bHalfOpenFile(pos.whitePawns, pos.blackPawns);

        int positional = __builtin_popcountll(pos.blackPawns & center) * 40;  // occupying the focal center
        positional += __builtin_popcountll((pos.blackBishops | pos.blackKnights) & center) * 20;  // light pieces
        positional += __builtin_popcountll(pos.blackQueens & center) * 30;  // queen
        positional += __builtin_popcountll(bPawnAllAttacks(pos.blackPawns) & advancedCenter) * 10;
        positional += __builtin_popcountll(((pos.blackPieces) & (~pos.blackKings)) & advancedOnlyCenter) * 10;

        Bitboard isQueenOnOFile = aOpenFiles & pos.blackQueens;
        Bitboard isQueenOnHFile = bHalfOpenFiles & pos.blackQueens;
        if (isQueenOnOFile) {
            if ((isQueenOnOFile & pos.whiteKings) != 0) {
                positional += 20;
            } else {
                positional += 15;
            }
        } else if (isQueenOnHFile) {
            positional += 10;
        }

        Bitboard isRookOnOFile = aOpenFiles & pos.blackRooks;
        Bitboard isRookOnHFile = bHalfOpenFiles & pos.blackRooks;
        positional += 15 * __builtin_popcountll(isRookOnHFile);
        positional += 3 * __builtin_popcountll(isRookOnOFile);

        pos.score = -(pos.score + (positional + pos.blackPositional_tmp - blackPositional) + moveEval);
        pos.blackPositional = pos.blackPositional_tmp + positional;
    }
    pos.isMate = ((pos.whiteKings == 0) || (pos.blackKings == 0));
//    pos.score = -(pos.score + moveEval);

    return pos;
}

Position Position::nullMove() noexcept {
    Position pos = *this;
    pos.white2play = !pos.white2play;
    pos.enpassant = 0x0;
    pos.score = -pos.score;
    pos.moves = pos.genMoves();
    return pos;
}

bool Position::operator<(const Position &pos) const {
//    if (white2play < pos.white2play) return true;
//    if (enpassant < pos.enpassant) return true;
//    if (whitePawns < pos.whitePawns) return true;
//    if (whiteKnights < pos.whiteKnights) return true;
//    if (whiteBishops < pos.whiteBishops) return true;
//    if (whiteRooks < pos.whiteRooks) return true;
//    if (whiteQueens < pos.whiteQueens) return true;
//    if (whiteKings < pos.whiteKings) return true;
//    if (blackPawns < pos.blackPawns) return true;
//    if (blackKnights < pos.blackKnights) return true;
//    if (blackBishops < pos.blackBishops) return true;
//    if (blackRooks < pos.blackRooks) return true;
//    if (blackQueens < pos.blackQueens) return true;
//    if (blackKings < pos.blackKings) return true;
//    if (whiteKingSideCastle < pos.whiteKingSideCastle) return true;
//    if (whiteQueenSideCastle < pos.whiteQueenSideCastle) return true;
//    if (blackKingSideCastle < pos.blackKingSideCastle) return true;
//    if (blackQueenSideCastle < pos.blackQueenSideCastle) return true;
//    return false;
    return board < pos.board;
}

std::vector<std::pair<Move, int>> Position::genMoves() noexcept {
    std::vector<Move> temp;
    temp.reserve(218);

    bool isEndgame = overallScore <= 3000;
    int bonus;
    if (white2play) {
        bonus = allMoves(whitePawns, whiteKnights, whiteBishops, whiteRooks, whiteQueens, whiteKings,
                         enpassant, blackPieces, whitePieces, whiteQueenSideCastle, whiteKingSideCastle,
                         white2play, isEndgame, temp);
    } else {
        bonus = allMoves(blackPawns, blackKnights, blackBishops, blackRooks, blackQueens, blackKings,
                         enpassant, whitePieces, blackPieces, blackQueenSideCastle, blackKingSideCastle,
                         white2play, isEndgame, temp);
    }

    std::vector<std::pair<Move, int>> res;
    res.reserve(temp.size());
    for (const auto& move: temp) {
        res.emplace_back(move, value(move));
    }

    if (white2play) {
        whitePositional_tmp = bonus;
    } else {
        blackPositional_tmp = bonus;
    }
    return res;
}

std::vector<Move> Position::genMovesFast() const noexcept {
    std::vector<Move> temp;
    temp.reserve(218);

    bool isEndgame = overallScore <= 3000;
    int bonus;
    if (white2play) {
        bonus = allMoves(whitePawns, whiteKnights, whiteBishops, whiteRooks, whiteQueens, whiteKings,
                         enpassant, blackPieces, whitePieces, whiteQueenSideCastle, whiteKingSideCastle,
                         white2play, isEndgame, temp);
    } else {
        bonus = allMoves(blackPawns, blackKnights, blackBishops, blackRooks, blackQueens, blackKings,
                         enpassant, whitePieces, blackPieces, blackQueenSideCastle, blackKingSideCastle,
                         white2play, isEndgame, temp);
    }

    return temp;
}

int Position::value(Move move) const noexcept {
    uint64_t i = move.src;
    uint64_t j = move.dst;
    char p = board[i];
    char q = board[j];

    int enpasOffset = -8;

    int staticEval = whitePositional;

    if (!white2play) {
        p -= 32;
        q += 32;

        uint64_t rank = j >> 3;
        if (rank >= 4) {
            j = j - (8 + ((rank - 4) << 4));
        } else {
            j = j + (8 + ((3 - rank) << 4));
        }

        rank = i >> 3;
        if (rank >= 4) {
            i = i - (8 + ((rank - 4) << 4));
        } else {
            i = i + (8 + ((3 - rank) << 4));
        }
        enpasOffset = 8;
        staticEval = blackPositional;
    }

    int scoreCopy = getTable(p, static_cast<int>(j), overallScore, pawns
    ) - getTable(p, static_cast<int>(i), overallScore, pawns
    );

    // capture
    if (move.isCapture) {
        uint64_t rank = j >> 3;
        uint64_t reversed;
        if (rank >= 4) {
            reversed = j - (8 + ((rank - 4) << 4));
        } else {
            reversed = j + (8 + ((3 - rank) << 4));
        }
        if (move.enpassant) {
            scoreCopy += getTable('P', static_cast<int>(reversed) - enpasOffset, overallScore, pawns);
        } else {
            scoreCopy += getTable(q - 32, static_cast<int>(reversed), overallScore, pawns);
        }
    }
    if (move.isCastleQueenSide | move.isCastleKingSide) {
        scoreCopy += getTable('R', static_cast<int>((i + j) >> 1), overallScore, pawns);
        scoreCopy -= getTable('R', j < i? 0: 7, overallScore, pawns);
    }

    if (move.promotion) {
        scoreCopy += getTable(
                'Q', static_cast<int>(j), overallScore, pawns
        ) - getTable('P', static_cast<int>(j), overallScore, pawns);
    }
    return scoreCopy;
}

inline __attribute__((always_inline)) int Position::getTable(char p, int ind, int rate, int pawns) noexcept {
    if (rate > 3000) {
        return pstMG[p][pawns][ind];
    } else {
        return pstEG[p][pawns][ind];
    }
}

void Position::fillTables() {
    pstMG.resize(83);
    pstEG.resize(83);
    for (std::pair<char, std::vector<int>> pair: pstTMG) {
        pstMG[pair.first].resize(17);
        for (int t = 0; t <= 16; ++t) {
            if (13 <= t) {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.9);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.85);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstMG[pair.first][t] = pair.second;
            } else if (9 <= t) {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.95);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.9);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else if (pair.first == 'B') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.05);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstMG[pair.first][t] = pair.second;
            } else if (5 <= t) {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.2);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.9);
                    } else if (pair.first == 'B') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.15);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstMG[pair.first][t] = pair.second;
            } else {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.3);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.85);
                    } else if (pair.first == 'B') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.2);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstMG[pair.first][t] = pair.second;
            }
        }
    }

    for (std::pair<char, std::vector<int>> pair: pstTEG) {
        pstEG[pair.first].resize(17);
        for (int t = 0; t <= 16; ++t) {
            if (13 <= t) {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.9);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.85);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstEG[pair.first][t] = pair.second;
            } else if (9 <= t) {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.95);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.9);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else if (pair.first == 'B') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.05);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstEG[pair.first][t] = pair.second;
            } else if (5 <= t) {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.2);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.9);
                    } else if (pair.first == 'B') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.15);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstEG[pair.first][t] = pair.second;
            } else {
                for (int i = 0; i < 64; ++i) {
                    if (pair.first == 'Q') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.3);
                    } else if (pair.first == 'R') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.1);
                    } else if (pair.first == 'N') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 0.85);
                    } else if (pair.first == 'B') {
                        pair.second[i] += static_cast<int>(piece[pair.first] * 1.2);
                    } else {
                        pair.second[i] += piece[pair.first];
                    }
                }
                pstEG[pair.first][t] = pair.second;
            }
        }
    }
}

std::vector<std::vector<std::vector<int>>> pstMG, pstEG;
SliderAttacks slider_attacks;
