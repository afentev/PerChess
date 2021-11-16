//
// Created by afentev on 5/30/21.
//

#ifndef PERCHESS_POSITION_H
#define PERCHESS_POSITION_H

#include <cstdint>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <thread>

#include "SlidingBitboardsFiles/slider_attacks.h"
#include "PawnsBitboardsFiles/PawnsAttacks.h"
#include "KnightsBitboardsFiles/KnightsMoves.h"
#include "KingBitboardsFiles/KingMoves.h"
#include "MobilityFiles/Mobility.h"
#include "Measures.h"
#include "Tables.h"
#include "MovesJumpTable.h"

#define Bitboard uint64_t
const Bitboard notAFile = 0xfefefefefefefefe;
const Bitboard notHFile = 0x7f7f7f7f7f7f7f7f;
const Bitboard isLastRank = 0xff00000000000000;
const Bitboard notLastRank = 0xffffffffffffff;
const Bitboard isFirstRank = 0xff;
const Bitboard notFirstRank = 0xffffffffffffff00;
const Bitboard isPreLastRank = 0xff000000000000;
const Bitboard isSecondRank = 0xff00;
const Bitboard isThirdRank = 0xff0000;
const Bitboard isThirdRankBack = 0xff0000000000;
const Bitboard center = 0x0000001818000000;
const Bitboard advancedCenter = 0x00003C3C3C3C0000;
const Bitboard advancedOnlyCenter = center ^ advancedCenter;
const int NORTH = 8;
const int SOUTH = -8;
const int EAST = 1;
const int WEST = -1;

extern std::set<char> upper;
extern std::set<char> lower;
extern std::vector<std::vector<std::vector<int>>> pstMG, pstEG;
extern SliderAttacks slider_attacks;


struct Move {
    Move(int p, uint64_t source, uint64_t destination, bool capture) {
        src = source;
        dst = destination;
        isCapture = capture;
        piece = static_cast<int_fast8_t> (p);
    }

    Move(int p, uint64_t source, uint64_t destination, bool capture, bool isCastleQ, bool isCastleK) {
        src = source;
        dst = destination;
        isCapture = capture;
        isCastleKingSide = isCastleK;
        isCastleQueenSide = isCastleQ;
        piece = static_cast<int_fast8_t> (p);
    }

    Move(int p, uint64_t source, uint64_t destination, bool capture, int prom) {
        src = source;
        dst = destination;
        isCapture = capture;
        promotion = static_cast<int_fast8_t>(prom);
        piece = static_cast<int_fast8_t> (p);
    }

    Move(int p, uint64_t source, uint64_t destination) {
        src = source;
        dst = destination;
        piece = static_cast<int_fast8_t> (p);
    }

    Move() {
        src = dst = 0xFFFFFFFFFFFFFFFF;
    }

    uint64_t src, dst;
    bool isCapture = false;
    bool isCastleQueenSide = false;
    bool isCastleKingSide = false;
    bool enpassant = false;

    int_fast8_t promotion = 0;
    /*
     * 0 - no promotion
     * 1 - promotion to a queen
     * 2 - promotion to a rook
     * 3 - promotion to a bishop
     * 4 - promotion to a knight
     */

    int_fast8_t piece = -1;
    /*
     * 1 - pawn
     * 2 - rook
     * 3 - knight
     * 4 - bishop
     * 5 - queen
     * 6 - king
     */
};

class Position {
public:
    bool white2play = true;

    std::string board;

    Bitboard whitePieces, blackPieces;
    Bitboard occupied, empty;
    Bitboard whitePawns, blackPawns;
    Bitboard whiteKnights, blackKnights;
    Bitboard whiteBishops, blackBishops;
    Bitboard whiteRooks, blackRooks;
    Bitboard whiteQueens, blackQueens;
    Bitboard whiteKings, blackKings;
    Bitboard enpassant = 0x0;

    bool whiteKingSideCastle = true;
    bool whiteQueenSideCastle = true;
    bool blackKingSideCastle = true;
    bool blackQueenSideCastle= true;

    int_fast32_t score = 0;
    int_fast32_t overallScore = 0;
    int_fast32_t whitePositional = 0;
    int_fast32_t blackPositional = 0;
    int_fast32_t whitePositional_tmp = 0;
    int_fast32_t blackPositional_tmp = 0;

    int_fast32_t pawns = 16;

    std::vector<std::pair<Move, int>> moves;

    bool isMate = false;

    void Initialize();

    static Bitboard eastOne (Bitboard b) noexcept;
    static Bitboard noEaOne (Bitboard b) noexcept;
    static Bitboard soEaOne (Bitboard b) noexcept;
    static Bitboard westOne (Bitboard b) noexcept;
    static Bitboard soWeOne (Bitboard b) noexcept;
    static Bitboard noWeOne (Bitboard b) noexcept;

    static Bitboard wPawnEastAttacks(Bitboard wpawns) noexcept;
    static Bitboard wPawnWestAttacks(Bitboard wpawns) noexcept;
    static Bitboard wPawnAllAttacks(Bitboard wpawns) noexcept;

    static Bitboard bPawnEastAttacks(Bitboard bpawns) noexcept;
    static Bitboard bPawnWestAttacks(Bitboard bpawns) noexcept;
    static Bitboard bPawnAllAttacks(Bitboard bpawns) noexcept;

    static void pawnNonCaptures(Bitboard pawns, Bitboard enemies, Bitboard friendly,
                         bool side2play, std::vector<Move>& res) noexcept;

    static void pawnCaptures(Bitboard pawns, Bitboard enemies, Bitboard enpas,
                      bool side2play, std::vector<Move>& res) noexcept;

    static void pawnMoves(Bitboard pawns, Bitboard enemies, Bitboard friendly,
                   Bitboard enpas, bool side2play, std::vector<Move>& res) noexcept;

    static int rookMoves(Bitboard rooks, Bitboard enemies, Bitboard friendly, bool isEndgame,
                         std::vector<Move>& res, int pieceType = 2) noexcept;

    static int bishopMoves(Bitboard bishops, Bitboard enemies, Bitboard friendly, bool isEndgame,
                           std::vector<Move>& res, int pieceType = 4) noexcept;

    static int queenMoves(Bitboard queens, Bitboard enemies, Bitboard friendly,
                          bool isEndgame, std::vector<Move>& res) noexcept;

    static int knightMoves(Bitboard knights, Bitboard enemies, Bitboard friendly,
                           bool isEndgame, std::vector<Move>& res) noexcept;

    static void kingMoves(Bitboard kings, Bitboard enemies, Bitboard friendly, Bitboard friendlyRooks,
                          bool qcastle, bool kcastle, bool side2play, std::vector<Move>& res) noexcept;

    static int allMoves(Bitboard pawns, Bitboard knights, Bitboard bishops,
                         Bitboard rooks, Bitboard queens, Bitboard kings,
                         Bitboard enpas,
                         Bitboard enemies, Bitboard friendly,
                         bool qcastle, bool kcastle, bool side2play,
                         bool isEndgame, std::vector<Move>& res) noexcept;

    std::vector<std::pair<Move, int>> genMoves() noexcept;
    std::vector<Move> genMovesFast() const noexcept;

    void makeCapture(Move& move, uint64_t destination,
                            uint64_t& appendixForEnemies, Bitboard& enemiesQueens, Bitboard& enemiesBishops,
                            Bitboard& enemiesKnights, Bitboard& enemiesRooks, Bitboard& enemiesPawns,
                            Bitboard& enemiesKings, bool& qcastle, bool& kcastle, int enpasOffset = 0) noexcept;

    Position playMoveCopy(Move move, bool side2move) const noexcept;

    Position nullMove() noexcept;

    static int getTable(char p, int ind, int rate, int pawns) noexcept;

    int value(Move move) const noexcept;

    static void fillTables();

    bool operator==(const Position& pos) const {
        return (white2play == pos.white2play && enpassant == pos.enpassant &&
                whitePawns == pos.whitePawns && whiteKnights == pos.whiteKnights &&
                whiteBishops == pos.whiteBishops && whiteRooks == pos.whiteRooks &&
                whiteQueens == pos.whiteQueens && whiteKings == pos.whiteKings &&
                blackPawns == pos.blackPawns && blackKnights == pos.blackKnights &&
                blackBishops == pos.blackBishops && blackRooks == pos.blackRooks &&
                blackQueens == pos.blackQueens && blackKings == pos.blackKings &&
                whiteKingSideCastle == pos.whiteKingSideCastle &&
                whiteQueenSideCastle == pos.whiteQueenSideCastle &&
                blackKingSideCastle == pos.blackKingSideCastle &&
                blackQueenSideCastle == pos.blackQueenSideCastle);
    };

    bool operator<(const Position& pos) const;
};

namespace std
{
    template<> struct hash<Position>
    {
        std::size_t operator()(Position const& t) const noexcept
        {
            size_t hash = 17;
            hash = hash * 31 + std::hash<uint64_t>()(t.whitePawns);
            hash = hash * 31 + std::hash<uint64_t>()(t.whiteKings);
            hash = hash * 31 + std::hash<uint64_t>()(t.whiteQueens);
            hash = hash * 31 + std::hash<uint64_t>()(t.whiteRooks);
            hash = hash * 31 + std::hash<uint64_t>()(t.whiteBishops);
            hash = hash * 31 + std::hash<uint64_t>()(t.whiteKnights);
            hash = hash * 31 + std::hash<uint64_t>()(t.whitePieces);

            hash = hash * 31 + std::hash<uint64_t>()(t.blackPawns);
            hash = hash * 31 + std::hash<uint64_t>()(t.blackKings);
            hash = hash * 31 + std::hash<uint64_t>()(t.blackQueens);
            hash = hash * 31 + std::hash<uint64_t>()(t.blackRooks);
            hash = hash * 31 + std::hash<uint64_t>()(t.blackBishops);
            hash = hash * 31 + std::hash<uint64_t>()(t.blackKnights);
            hash = hash * 31 + std::hash<uint64_t>()(t.blackPieces);
            return hash;
        }
    };
}

typedef std::tuple<Position, int, bool> key;

struct key_hash : public std::unary_function<key, std::size_t>
{
    std::size_t operator()(const key& k) const
    {
        return std::hash<Position>()(std::get<0>(k)) ^ std::hash<int>()(std::get<1>(k)) ^
                std::hash<bool>()(std::get<2>(k));
    }
};


//class A: public Position {
//virtual volatile inline const constexpr std::vector<const uint_fast32_t* const&&>* const& vec(register void**const*** const& __restrict__ x) final override;
//};


#endif //PERCHESS_POSITION_H
