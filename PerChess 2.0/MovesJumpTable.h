//
// Created by afentev on 6/13/21.
//

#ifndef PERCHESS_MOVESJUMPTABLE_H
#define PERCHESS_MOVESJUMPTABLE_H

#include <vector>

class Position;
struct Move;


void WhitePawnCaptureNonPromotion(Position& pos, Move& move);
void WhitePawnCaptureAndPromotion(Position& pos, Move& move);
void WhitePawnNonCaptureAndPromotion(Position& pos, Move& move);
void WhitePawnNonCaptureNonPromotion(Position& pos, Move& move);
void WhiteBishopCapture(Position& pos, Move& move);
void WhiteBishopNonCapture(Position& pos, Move& move);
void WhiteKnightCapture(Position& pos, Move& move);
void WhiteKnightNonCapture(Position& pos, Move& move);
void WhiteRookCapture(Position& pos, Move& move);
void WhiteRookNonCapture(Position& pos, Move& move);
void WhiteQueenCapture(Position& pos, Move& move);
void WhiteQueenNonCapture(Position& pos, Move& move);
void WhiteKingCapture(Position& pos, Move& move);
void WhiteKingNonCaptureKingCastle(Position& pos, Move& move);
void WhiteKingNonCaptureQueenCastle(Position& pos, Move& move);
void WhiteKingNonCaptureNonCastle(Position& pos, Move& move);

void BlackPawnCaptureNonPromotion(Position& pos, Move& move);
void BlackPawnCaptureAndPromotion(Position& pos, Move& move);
void BlackPawnNonCaptureAndPromotion(Position& pos, Move& move);
void BlackPawnNonCaptureNonPromotion(Position& pos, Move& move);
void BlackBishopCapture(Position& pos, Move& move);
void BlackBishopNonCapture(Position& pos, Move& move);
void BlackKnightCapture(Position& pos, Move& move);
void BlackKnightNonCapture(Position& pos, Move& move);
void BlackRookCapture(Position& pos, Move& move);
void BlackRookNonCapture(Position& pos, Move& move);
void BlackQueenCapture(Position& pos, Move& move);
void BlackQueenNonCapture(Position& pos, Move& move);
void BlackKingCapture(Position& pos, Move& move);
void BlackKingNonCaptureKingCastle(Position& pos, Move& move);
void BlackKingNonCaptureQueenCastle(Position& pos, Move& move);
void BlackKingNonCaptureNonCastle(Position& pos, Move& move);


extern std::vector<std::vector<std::vector<std::vector<void (*)(Position& pos, Move& move)>>>> jumpTable;

#endif //PERCHESS_MOVESJUMPTABLE_H
