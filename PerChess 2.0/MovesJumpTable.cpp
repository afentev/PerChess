//
// Created by afentev on 6/13/21.
//

#include "MovesJumpTable.h"
#include "Position.h"

inline __attribute__((always_inline)) void WhitePawnCaptureNonPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    int enpasOffset = -8;
    uint64_t appendix = 0;
    pos.makeCapture(move, dst, appendix, pos.blackQueens, pos.blackBishops,
                    pos.blackKnights, pos.blackRooks, pos.blackPawns, pos.blackKings,
                    pos.blackQueenSideCastle, pos.blackKingSideCastle, enpasOffset);

    pos.whitePawns ^= 1ull << move.src;
    pos.whitePawns |= dst;

    if (move.enpassant) {
        pos.board[move.dst + enpasOffset] = ' ';
        pos.overallScore += piece[static_cast<char>(pos.board[move.dst] - appendix)] -
                            piece['P'];
    }
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhitePawnCaptureAndPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 0;
    pos.makeCapture(move, dst, appendix, pos.blackQueens, pos.blackBishops,
                    pos.blackKnights, pos.blackRooks, pos.blackPawns, pos.blackKings,
                    pos.blackQueenSideCastle, pos.blackKingSideCastle);

    pos.whitePawns ^= src;
    if (move.promotion == 1) {
        pos.board[move.dst] = 'Q';
        pos.whiteQueens |= dst;
    } else if (move.promotion == 2) {
        pos.board[move.dst] = 'R';
        pos.whiteRooks |= dst;
    } else if (move.promotion == 3) {
        pos.board[move.dst] = 'B';
        pos.whiteBishops |= dst;
    } else if (move.promotion == 4) {
        pos.board[move.dst] = 'N';
        pos.whiteKnights |= dst;
    }
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhitePawnNonCaptureAndPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    if (move.promotion == 1) {
        pos.board[move.dst] = 'Q';
        pos.whiteQueens |= dst;
    } else if (move.promotion == 2) {
        pos.board[move.dst] = 'R';
        pos.whiteRooks |= dst;
    } else if (move.promotion == 3) {
        pos.board[move.dst] = 'B';
        pos.whiteBishops |= dst;
    } else if (move.promotion == 4) {
        pos.board[move.dst] = 'N';
        pos.whiteKnights |= dst;
    }
    pos.board[move.src] = ' ';
    pos.whitePawns ^= src;
}

inline __attribute__((always_inline)) void WhitePawnNonCaptureNonPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    
    // basic move
    if (move.dst - move.src == 16ull) {
        pos.enpassant = 1ull << (move.src + 8ull);
    }
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
    pos.whitePawns ^= src;
    pos.whitePawns |= dst;
}

inline __attribute__((always_inline)) void WhiteBishopCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 0;

    pos.makeCapture(move, dst, appendix, pos.blackQueens, pos.blackBishops,
                    pos.blackKnights, pos.blackRooks, pos.blackPawns, pos.blackKings,
                    pos.blackQueenSideCastle, pos.blackKingSideCastle);
    pos.whiteBishops ^= src;
    pos.whiteBishops |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteBishopNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    pos.whiteBishops ^= src;
    pos.whiteBishops |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteKnightCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 0;

    pos.makeCapture(move, dst, appendix, pos.blackQueens, pos.blackBishops,
                    pos.blackKnights, pos.blackRooks, pos.blackPawns, pos.blackKings,
                    pos.blackQueenSideCastle, pos.blackKingSideCastle);
    
    pos.whiteKnights ^= src;
    pos.whiteKnights |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteKnightNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    pos.whiteKnights ^= src;
    pos.whiteKnights |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteRookCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 0;

    pos.makeCapture(move, dst, appendix, pos.blackQueens, pos.blackBishops,
                    pos.blackKnights, pos.blackRooks, pos.blackPawns, pos.blackKings,
                    pos.blackQueenSideCastle, pos.blackKingSideCastle);
    if (move.src == 0ull) {
        pos.whiteQueenSideCastle = false;
    } else if (move.src == 7ull) {
        pos.whiteKingSideCastle = false;
    }
    pos.whiteRooks ^= src;
    pos.whiteRooks |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteRookNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    
    if (move.src == 0ull) {
        pos.whiteQueenSideCastle = false;
    } else if (move.src == 7ull) {
        pos.whiteKingSideCastle = false;
    }
    pos.whiteRooks ^= src;
    pos.whiteRooks |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteQueenCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 0;

    pos.makeCapture(move, dst, appendix, pos.blackQueens, pos.blackBishops,
                    pos.blackKnights, pos.blackRooks, pos.blackPawns, pos.blackKings,
                    pos.blackQueenSideCastle, pos.blackKingSideCastle);
    pos.whiteQueens ^= src;
    pos.whiteQueens |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteQueenNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    pos.whiteQueens ^= src;
    pos.whiteQueens |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteKingCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 0;

    pos.makeCapture(move, dst, appendix, pos.blackQueens, pos.blackBishops,
                    pos.blackKnights, pos.blackRooks, pos.blackPawns, pos.blackKings,
                    pos.blackQueenSideCastle, pos.blackKingSideCastle);
    
    pos.whiteKingSideCastle = pos.whiteQueenSideCastle = false;

    pos.whiteKings ^= src;
    pos.whiteKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteKingNonCaptureKingCastle(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    
    pos.whiteKingSideCastle = pos.whiteQueenSideCastle = false;
    pos.whiteRooks = pos.whiteRooks & ~(1ull << 7ull) | (1ull << 5ull);  // TODO
    pos.board[7] = ' ';
    pos.board[5] = 'R';
    
    pos.whiteKings ^= src;
    pos.whiteKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteKingNonCaptureQueenCastle(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;

    pos.whiteKingSideCastle = pos.whiteQueenSideCastle = false;
    pos.whiteRooks = pos.whiteRooks & ~(1ull << 0ull) | (1ull << 3ull);  // TODO
    pos.board[0] = ' ';
    pos.board[3] = 'R';

    pos.whiteKings ^= src;
    pos.whiteKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void WhiteKingNonCaptureNonCastle(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    
    pos.whiteKingSideCastle = pos.whiteQueenSideCastle = false;
    pos.whiteKings ^= src;
    pos.whiteKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline __attribute__((always_inline)) void BlackPawnCaptureNonPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    int enpasOffset = 8;
    uint64_t appendix = 32;
    pos.makeCapture(move, dst, appendix, pos.whiteQueens, pos.whiteBishops,
                    pos.whiteKnights, pos.whiteRooks, pos.whitePawns, pos.whiteKings,
                    pos.whiteQueenSideCastle, pos.whiteKingSideCastle, enpasOffset);

    pos.blackPawns ^= 1ull << move.src;
    pos.blackPawns |= dst;

    if (move.enpassant) {
        pos.board[move.dst + enpasOffset] = ' ';
        pos.overallScore += piece[static_cast<char>(pos.board[move.dst] - appendix)] -
                            piece['P'];
    }
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackPawnCaptureAndPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 32;
    pos.makeCapture(move, dst, appendix, pos.whiteQueens, pos.whiteBishops,
                    pos.whiteKnights, pos.whiteRooks, pos.whitePawns, pos.whiteKings,
                    pos.whiteQueenSideCastle, pos.whiteKingSideCastle);

    pos.blackPawns ^= src;
    if (move.promotion == 1) {
        pos.board[move.dst] = 'q';
        pos.blackQueens |= dst;
    } else if (move.promotion == 2) {
        pos.board[move.dst] = 'r';
        pos.blackRooks |= dst;
    } else if (move.promotion == 3) {
        pos.board[move.dst] = 'b';
        pos.blackBishops |= dst;
    } else if (move.promotion == 4) {
        pos.board[move.dst] = 'n';
        pos.blackKnights |= dst;
    }
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackPawnNonCaptureAndPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    if (move.promotion == 1) {
        pos.board[move.dst] = 'q';
        pos.blackQueens |= dst;
    } else if (move.promotion == 2) {
        pos.board[move.dst] = 'r';
        pos.blackRooks |= dst;
    } else if (move.promotion == 3) {
        pos.board[move.dst] = 'b';
        pos.blackBishops |= dst;
    } else if (move.promotion == 4) {
        pos.board[move.dst] = 'n';
        pos.blackKnights |= dst;
    }
    pos.board[move.src] = ' ';
    pos.blackPawns ^= src;
}

inline __attribute__((always_inline)) void BlackPawnNonCaptureNonPromotion(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;

    // basic move
    if (move.src - move.dst == 16ull) {
        pos.enpassant = 1ull << (move.dst + 8ull);
    }
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
    pos.blackPawns ^= src;
    pos.blackPawns |= dst;
}

inline __attribute__((always_inline)) void BlackBishopCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 32;

    pos.makeCapture(move, dst, appendix, pos.whiteQueens, pos.whiteBishops,
                    pos.whiteKnights, pos.whiteRooks, pos.whitePawns, pos.whiteKings,
                    pos.whiteQueenSideCastle, pos.whiteKingSideCastle);
    pos.blackBishops ^= src;
    pos.blackBishops |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackBishopNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    pos.blackBishops ^= src;
    pos.blackBishops |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackKnightCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 32;

    pos.makeCapture(move, dst, appendix, pos.whiteQueens, pos.whiteBishops,
                    pos.whiteKnights, pos.whiteRooks, pos.whitePawns, pos.whiteKings,
                    pos.whiteQueenSideCastle, pos.whiteKingSideCastle);

    pos.blackKnights ^= src;
    pos.blackKnights |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackKnightNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    pos.blackKnights ^= src;
    pos.blackKnights |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackRookCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 32;

    pos.makeCapture(move, dst, appendix, pos.whiteQueens, pos.whiteBishops,
                    pos.whiteKnights, pos.whiteRooks, pos.whitePawns, pos.whiteKings,
                    pos.whiteQueenSideCastle, pos.whiteKingSideCastle);
    if (move.src == 56ull) {
        pos.blackQueenSideCastle = false;
    } else if (move.src == 63ull) {
        pos.blackKingSideCastle = false;
    }
    pos.blackRooks ^= src;
    pos.blackRooks |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackRookNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;

    if (move.src == 56ull) {
        pos.blackQueenSideCastle = false;
    } else if (move.src == 63ull) {
        pos.blackKingSideCastle = false;
    }
    pos.blackRooks ^= src;
    pos.blackRooks |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackQueenCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 32;

    pos.makeCapture(move, dst, appendix, pos.whiteQueens, pos.whiteBishops,
                    pos.whiteKnights, pos.whiteRooks, pos.whitePawns, pos.whiteKings,
                    pos.whiteQueenSideCastle, pos.whiteKingSideCastle);
    pos.blackQueens ^= src;
    pos.blackQueens |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackQueenNonCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    pos.blackQueens ^= src;
    pos.blackQueens |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackKingCapture(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;
    uint64_t appendix = 32;

    pos.makeCapture(move, dst, appendix, pos.whiteQueens, pos.whiteBishops,
                    pos.whiteKnights, pos.whiteRooks, pos.whitePawns, pos.whiteKings,
                    pos.whiteQueenSideCastle, pos.whiteKingSideCastle);

    pos.blackKingSideCastle = pos.blackQueenSideCastle = false;

    pos.blackKings ^= src;
    pos.blackKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackKingNonCaptureKingCastle(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;

    pos.blackKingSideCastle = pos.blackQueenSideCastle = false;
    pos.blackRooks = (pos.blackRooks & ~(1ull << 63ull)) | (1ull << 61ull);  // TODO
    pos.board[63] = ' ';
    pos.board[61] = 'r';

    pos.blackKings ^= src;
    pos.blackKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackKingNonCaptureQueenCastle(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;

    pos.blackKingSideCastle = pos.blackQueenSideCastle = false;
    pos.blackRooks = (pos.blackRooks & ~(1ull << 56ull)) | (1ull << 59ull);  // TODO
    pos.board[56] = ' ';
    pos.board[59] = 'r';

    pos.blackKings ^= src;
    pos.blackKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}

inline __attribute__((always_inline)) void BlackKingNonCaptureNonCastle(Position& pos, Move& move) {
    uint64_t dst = 1ull << move.dst;
    uint64_t src = 1ull << move.src;

    pos.blackKingSideCastle = pos.blackQueenSideCastle = false;
    pos.blackKings ^= src;
    pos.blackKings |= dst;
    pos.board[move.dst] = pos.board[move.src];
    pos.board[move.src] = ' ';
}


std::vector<std::vector<std::vector<std::vector<void (*)(Position& pos, Move& move)>>>> jumpTable = {
        {
                {  // pawn
                        {  // isCapture?
                                // isPromotion?
                                BlackPawnNonCaptureNonPromotion,
                                BlackPawnNonCaptureAndPromotion
                        },
                        {
                                BlackPawnCaptureNonPromotion,
                                BlackPawnCaptureAndPromotion
                        }
                },

                {  // roook
                        // isCapture?
                        {
                                BlackRookNonCapture
                        },
                        {
                                BlackRookCapture
                        }
                },

                {  // knight
                        {
                                BlackKnightNonCapture
                        },
                        {
                                BlackKnightCapture
                        }
                },

                {  // bishop
                        {
                                BlackBishopNonCapture
                        },
                        {
                                BlackBishopCapture
                        }
                },

                {  // queen
                        {
                                BlackQueenNonCapture
                        },
                        {
                                BlackQueenCapture
                        }
                },

                {  // king
                        {
                                BlackKingNonCaptureNonCastle,
                                BlackKingNonCaptureKingCastle,
                                BlackKingNonCaptureQueenCastle
                        },
                        {
                                BlackKingCapture
                        }
                }
        },
        
        {
            {  // pawn
                 {  // isCapture?
                         // isPromotion?
                         WhitePawnNonCaptureNonPromotion,
                         WhitePawnNonCaptureAndPromotion
                 },
                 {
                         WhitePawnCaptureNonPromotion,
                         WhitePawnCaptureAndPromotion
                 }
         },

                {  // roook
                        // isCapture?
                        {
                                WhiteRookNonCapture
                        },
                        {
                                WhiteRookCapture
                        }
                },

                {  // knight
                        {
                                WhiteKnightNonCapture
                        },
                        {
                                WhiteKnightCapture
                        }
                },

                {  // bishop
                        {
                                WhiteBishopNonCapture
                        },
                        {
                                WhiteBishopCapture
                        }
                },

                {  // queen
                        {
                                WhiteQueenNonCapture
                        },
                        {
                                WhiteQueenCapture
                        }
                },

                {  // king
                        {
                                WhiteKingNonCaptureNonCastle,
                                WhiteKingNonCaptureKingCastle,
                                WhiteKingNonCaptureQueenCastle
                        },
                        {
                                WhiteKingCapture
                        }
                }
        }
};
