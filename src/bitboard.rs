use crate::board::{Move, Square};
use num_traits::PrimInt;

#[derive(Copy, Clone)]
pub struct Bitboard {
    board: u64,
}

impl Bitboard {
    pub const fn from_u64(bitboard: u64) -> Self {
        Self { board: bitboard }
    }

    pub fn make_move(&self, move_to_play: &Move) -> Bitboard {
        self.clear_square(move_to_play.source)
            .occupy_square(move_to_play.dst)
    }

    pub fn to_u64(&self) -> u64 {
        self.board
    }

    pub fn entries_count(&self) -> u8 {
        self.board.count_ones() as u8
    }

    pub fn get_first_entry(&mut self) -> Option<Square> {
        let result = self.board.trailing_zeros();
        if result < 64 {
            self.board ^= 1u64 << result; // clear taken bit
            Some(Square::create_unsafe(result as u8))
        } else {
            None
        }
    }

    pub fn up_one_rank(&self) -> Bitboard {
        Bitboard {
            board: self.board.unsigned_shl(8),
        }
    }

    pub fn filter_from(&self, occupied: &Bitboard) -> Bitboard {
        Bitboard {
            board: self.board & !occupied.board,
        }
    }

    pub fn filter_to(&self, occupied: &Bitboard) -> Bitboard {
        Bitboard {
            board: self.board & occupied.board,
        }
    }

    pub fn filter_on_third_rank(&self) -> Bitboard {
        Bitboard {
            board: self.board & Self::THIRD_RANK_MASK,
        }
    }

    pub fn is_occupied(&self, square: Square) -> bool {
        let result = self.board.unsigned_shr(square.discriminant() as u32) & 1;
        result != 0
    }

    pub fn clear_square(&self, square: Square) -> Bitboard {
        Bitboard {
            board: self.board & !(1u64 << square.discriminant()),
        }
    }

    pub fn occupy_square(&self, square: Square) -> Bitboard {
        Bitboard {
            board: self.board | (1u64 << square.discriminant()),
        }
    }

    pub fn flip_board(&self) -> Bitboard {
        let board = self.board;
        let b1 = board & 0b11111111;
        let b2 = (board >> 8) & 0b11111111;
        let b3 = (board >> 16) & 0b11111111;
        let b4 = (board >> 24) & 0b11111111;
        let b5 = (board >> 32) & 0b11111111;
        let b6 = (board >> 40) & 0b11111111;
        let b7 = (board >> 48) & 0b11111111;
        let b8 = (board >> 56) & 0b11111111;
        let new = b8 | (b7 << 8) | (b6 << 16) | (b5 << 24) | (b4 << 32) | (b3 << 40) | (b2 << 48) | (b1 << 56);
        Bitboard::from_u64(new)
    }

    const THIRD_RANK_MASK: u64 = 0b11111111_00000000_00000000u64;
    const SECOND_RANK_MASK: u64 = 0b11111111_00000000u64;
}
