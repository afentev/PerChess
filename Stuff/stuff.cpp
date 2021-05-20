//
// Created by afentev on 5/7/21.
//

#include "stuff.h"

std::random_device dev;
std::mt19937 rng;

int current_time_nanoseconds() {
    struct timespec tm{};
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_nsec;
}

int rand(int lhs, int rhs) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(lhs,rhs);
    return dist(rng);
}

static bool parse_FEN(struct pos *pos, const char *fen)
{
    uint64_t white = 0, black = 0;
    uint64_t kings, queens, rooks, bishops, knights, pawns;
    kings = queens = rooks = bishops = knights = pawns = 0;
    bool turn;
    uint32_t rule50 = 0, move = 1;
    uint32_t ep = 0;
    uint32_t castling = 0;
    char c;
    int r, f;

    if (fen == nullptr)
        goto fen_parse_error;

    for (r = 7; r >= 0; r--)
    {
        for (f = 0; f <= 7; f++)
        {
            uint32_t s = (r * 8) + f;
            uint64_t b = board(s);
            c = *fen++;
            switch (c)
            {
                case 'k':
                    kings |= b;
                    black |= b;
                    continue;
                case 'K':
                    kings |= b;
                    white |= b;
                    continue;
                case 'q':
                    queens |= b;
                    black |= b;
                    continue;
                case 'Q':
                    queens |= b;
                    white |= b;
                    continue;
                case 'r':
                    rooks |= b;
                    black |= b;
                    continue;
                case 'R':
                    rooks |= b;
                    white |= b;
                    continue;
                case 'b':
                    bishops |= b;
                    black |= b;
                    continue;
                case 'B':
                    bishops |= b;
                    white |= b;
                    continue;
                case 'n':
                    knights |= b;
                    black |= b;
                    continue;
                case 'N':
                    knights |= b;
                    white |= b;
                    continue;
                case 'p':
                    pawns |= b;
                    black |= b;
                    continue;
                case 'P':
                    pawns |= b;
                    white |= b;
                    continue;
                default:
                    break;
            }
            if (c >= '1' && c <= '8')
            {
                uint32_t jmp = (uint32_t)c - '0';
                f += jmp-1;
                continue;
            }
            goto fen_parse_error;
        }
        if (r == 0)
            break;
        c = *fen++;
        if (c != '/')
            goto fen_parse_error;
    }
    c = *fen++;
    if (c != ' ')
        goto fen_parse_error;
    c = *fen++;
    if (c != 'w' && c != 'b')
        goto fen_parse_error;
    turn = (c == 'w');
    c = *fen++;
    if (c != ' ')
        goto fen_parse_error;
    c = *fen++;
    if (c != '-')
    {
        do
        {
            switch (c)
            {
                case 'K':
                    castling |= TB_CASTLING_K; break;
                case 'Q':
                    castling |= TB_CASTLING_Q; break;
                case 'k':
                    castling |= TB_CASTLING_k; break;
                case 'q':
                    castling |= TB_CASTLING_q; break;
                default:
                    goto fen_parse_error;
            }
            c = *fen++;
        }
        while (c != ' ');
        fen--;
    }
    c = *fen++;
    if (c != ' ')
        goto fen_parse_error;
    c = *fen++;
    if (c >= 'a' && c <= 'h')
    {
        uint32_t file_ = c - 'a';
        c = *fen++;
        if (c != '3' && c != '6')
            goto fen_parse_error;
        uint32_t rank = c - '1';
        ep = square(rank, file_);
        if (rank == 2 && turn)
            goto fen_parse_error;
        if (rank == 5 && !turn)
            goto fen_parse_error;
        if (rank == 2 && ((tb_pawn_attacks(ep, true) & (black & pawns)) == 0))
            ep = 0;
        if (rank == 5 && ((tb_pawn_attacks(ep, false) & (white & pawns)) == 0))
            ep = 0;
    }
    else if (c != '-')
        goto fen_parse_error;
    c = *fen++;
    if (c != ' ')
        goto fen_parse_error;
    char clk[4];
    clk[0] = *fen++;
    if (clk[0] < '0' || clk[0] > '9')
        goto fen_parse_error;
    clk[1] = *fen++;
    if (clk[1] != ' ')
    {
        if (clk[1] < '0' || clk[1] > '9')
            goto fen_parse_error;
        clk[2] = *fen++;
        if (clk[2] != ' ')
        {
            if (clk[2] < '0' || clk[2] > '9')
                goto fen_parse_error;
            c = *fen++;
            if (c != ' ')
                goto fen_parse_error;
            clk[3] = '\0';
        }
        else
            clk[2] = '\0';
    }
    else
        clk[1] = '\0';
    rule50 = atoi(clk);
    move = atoi(fen);

    pos->white = white;
    pos->black = black;
    pos->kings = kings;
    pos->queens = queens;
    pos->rooks = rooks;
    pos->bishops = bishops;
    pos->knights = knights;
    pos->pawns = pawns;
    pos->castling = castling;
    pos->rule50 = rule50;
    pos->ep = ep;
    pos->turn = turn;
    pos->move = move;
    return true;

    fen_parse_error:
    return false;
}

void init() {
    rng = std::mt19937(dev());
    static struct option long_options[] =
            {
                    {"help",  0, nullptr, OPTION_HELP},
                    {"path",  1, nullptr, OPTION_PATH},
                    {"test",  0, nullptr, OPTION_TEST},
                    {nullptr, 0, nullptr, 0}
            };
    char *path = nullptr;
    bool test = false;

    // (0) init:
    path = getenv("TB_PATH");
    std::string p = "/Users/user/Downloads/3-4-5piecesSyzygy/3-4-5";
    path = &p[0];
    if (path == nullptr) {
        fprintf(stderr, "Path not set");
        exit(EXIT_FAILURE);
    }
    tb_init(path);
    if (TB_LARGEST == 0) {
        fprintf(stderr, "error: unable to initialize tablebase; no tablebase "
                        "files found\n");
        exit(EXIT_FAILURE);
    }
}

static bool is_en_passant(const struct pos *pos, uint64_t from, uint64_t to)
{
    uint64_t us = (pos->turn? pos->white: pos->black);
    if (pos->ep == 0)
        return false;
    if (to != pos->ep)
        return false;
    if ((board(from) & us & pos->pawns) == 0)
        return false;
    return true;
}

static void move_parts_to_str(const struct pos *pos, int from, int to, int promotes, char *str) {
    uint32_t r        = rank(from);
    uint32_t f        = file_(from);
    uint64_t occ      = pos->black | pos->white;
    uint64_t us       = (pos->turn? pos->white: pos->black);
    bool     capture  = (occ & board(to)) != 0 || is_en_passant(pos,from,to);
    uint64_t b = board(from), att = 0;
    if (b & pos->kings)
        *str++ = 'K';
    else if (b & pos->queens)
    {
        *str++ = 'Q';
        att = tb_queen_attacks(to, occ) & us & pos->queens;
    }
    else if (b & pos->rooks)
    {
        *str++ = 'R';
        att = tb_rook_attacks(to, occ) & us & pos->rooks;
    }
    else if (b & pos->bishops)
    {
        *str++ = 'B';
        att = tb_bishop_attacks(to, occ) & us & pos->bishops;
    }
    else if (b & pos->knights)
    {
        *str++ = 'N';
        att = tb_knight_attacks(to) & us & pos->knights;
    }
    else
        att = tb_pawn_attacks(to, !pos->turn) & us & pos->pawns;
    if ((b & pos->pawns) && capture)
        *str++ = 'a' + f;
    else if (tb_pop_count(att) > 1)
    {
        if (tb_pop_count(att & (BOARD_FILE_A >> f)) == 1)
            *str++ = 'a' + f;
        else if (tb_pop_count(att & (BOARD_RANK_1 << (8*r))) == 1)
            *str++ = '1' + r;
        else
        {
            *str++ = 'a' + f;
            *str++ = '1' + r;
        }
    }
    if (capture)
        *str++ = 'x';
    *str++ = 'a' + file_(to);
    *str++ = '1' + rank(to);
    if (promotes != TB_PROMOTES_NONE)
    {
        *str++ = '=';
        switch (promotes)
        {
            case TB_PROMOTES_QUEEN:
                *str++ = 'Q'; break;
            case TB_PROMOTES_ROOK:
                *str++ = 'R'; break;
            case TB_PROMOTES_BISHOP:
                *str++ = 'B'; break;
            case TB_PROMOTES_KNIGHT:
                *str++ = 'N'; break;
        }
    }
    *str++ = '\0';
}

static void move_to_str(const struct pos *pos, uint32_t move, char *str)
{
    uint32_t from     = TB_GET_FROM(move);
    uint32_t to       = TB_GET_TO(move);
    uint32_t promotes = TB_GET_PROMOTES(move);
    move_parts_to_str(pos, from, to, promotes, str);
}

static bool is_check(const struct pos *pos)
{
    uint64_t occ = pos->white | pos->black;
    uint64_t us = (pos->turn? pos->white: pos->black),
            them = (pos->turn? pos->black: pos->white);
    uint64_t king = pos->kings & us;
    unsigned sq = tb_lsb(king);
    uint64_t ratt = tb_rook_attacks(sq, occ);
    uint64_t batt = tb_bishop_attacks(sq, occ);
    if (ratt & (pos->rooks & them))
        return true;
    if (batt & (pos->bishops & them))
        return true;
    if ((ratt | batt) & (pos->queens & them))
        return true;
    if (tb_knight_attacks(sq) & (pos->knights & them))
        return true;
    if (tb_pawn_attacks(sq, pos->turn) & (pos->pawns & them))
        return true;
    return false;
}

static std::string print_PV(struct pos *pos) {
    unsigned move = tb_probe_root(pos->white, pos->black, pos->kings,
                                  pos->queens, pos->rooks, pos->bishops, pos->knights, pos->pawns,
                                  pos->rule50, pos->castling, pos->ep, pos->turn, nullptr);
    if (move == TB_RESULT_FAILED) {
        return "";
    }
    char str[32];
    move_to_str(pos, move, str);
    auto s = std::string(str);
    return str;
}

std::string proc(const char* fen) {
    struct pos pos0{};
    struct pos *pos = &pos0;
    if (!parse_FEN(pos, fen))
    {
        return "";
    }

    if (tb_pop_count(pos->white | pos->black) > TB_LARGEST)
    {
        return "";
    }
    uint32_t results[TB_MAX_MOVES];
    uint32_t res = tb_probe_root(pos->white, pos->black, pos->kings,
                                 pos->queens, pos->rooks, pos->bishops, pos->knights, pos->pawns,
                                 pos->rule50, pos->castling, pos->ep, pos->turn, results);
    if (res == TB_RESULT_FAILED)
    {
        return "";
    }

    return print_PV(pos);
}

std::string search(const std::string& pos) {
    return proc(pos.c_str());
}
