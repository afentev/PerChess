#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

typedef uint64_t U64;

static const int kSquares = 64;

static const char kRookMagics[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/rook_magics.magic";
static const char kRookMasks[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/rook_masks.magic";
static const char kRookShifts[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/rook_shifts.magic";
static const char kRookOffsets[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/rook_offsets.magic";
static const char kRookAttackTable[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/rook_attack_table.magic";

static const char kBishopMagics[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/bishop_magics.magic";
static const char kBishopMasks[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/bishop_masks.magic";
static const char kBishopShifts[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/bishop_shifts.magic";
static const char kBishopOffsets[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/bishop_offsets.magic";
static const char kBishopAttackTable[] =
        "/Users/user/CLionProjects/PerChess/SlidingBitboardsFiles/bishop_attack_table.magic";

#endif
