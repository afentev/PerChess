//
// Created by afentev on 3/24/21.
//

#ifndef PROJ_STUFF_H
#define PROJ_STUFF_H

#include <ctime>
#include <random>

#define BOARD_RANK_1            0x00000000000000FFull
#define BOARD_FILE_A            0x8080808080808080ull
#define square(r, f)            (8 * (r) + (f))
#define rank(s)                 ((s) >> 3)
#define file_(s)                 ((s) & 0x07)
#define board(s)                ((uint64_t)1 << (s))

#define OPTION_HELP     0
#define OPTION_PATH     1
#define OPTION_TEST     2

extern std::random_device dev;
extern std::mt19937 rng;

int current_time_nanoseconds();

int rand(int lhs, int rhs);

#include <cassert>
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
    #include "../fathom1/src/tbprobe.h"
}

struct pos
{
    uint64_t white;
    uint64_t black;
    uint64_t kings;
    uint64_t queens;
    uint64_t rooks;
    uint64_t bishops;
    uint64_t knights;
    uint64_t pawns;
    uint8_t castling;
    uint8_t rule50;
    uint8_t ep;
    bool turn;
    uint16_t move;
};

static bool parse_FEN(struct pos *pos, const char *fen);

void init();

static bool is_en_passant(const struct pos *pos, uint64_t from, uint64_t to);

static void move_parts_to_str(const struct pos *pos, int from, int to, int promotes, char *str);

static void move_to_str(const struct pos *pos, uint32_t move, char *str);

static bool is_check(const struct pos *pos);

static std::string print_PV(struct pos *pos);

std::string proc(const char* fen);


std::string search(const std::string& pos);


#endif //PROJ_STUFF_H
