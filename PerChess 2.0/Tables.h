//
// Created by afentev on 6/4/21.
//

#ifndef PERCHESS_TABLES_H
#define PERCHESS_TABLES_H

#include <map>
#include <vector>

extern std::map<char, std::vector<int>> pstTMG, pstTEG;
extern std::map<char, int> piece;
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


#endif //PERCHESS_TABLES_H
