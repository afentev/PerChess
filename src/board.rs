use crate::board::MoveType::{BishopCapture, EnPassant, KingCapture, KnightCapture, PawnCapture, PawnCapturePromotion, PawnQuiet, PawnQuietPromotion, QueenCapture, RookCapture};
use num_derive::FromPrimitive;
use num_traits::FromPrimitive;
use crate::internal_board::InternalBoard;

#[rustfmt::skip]
#[derive(FromPrimitive, Debug, Eq, PartialEq, Copy, Clone)]
#[repr(u8)]
pub enum Square {
    A8 = 56, B8 = 57, C8 = 58, D8 = 59, E8 = 60, F8 = 61, G8 = 62, H8 = 63,
    A7 = 48, B7 = 49, C7 = 50, D7 = 51, E7 = 52, F7 = 53, G7 = 54, H7 = 55,
    A6 = 40, B6 = 41, C6 = 42, D6 = 43, E6 = 44, F6 = 45, G6 = 46, H6 = 47,
    A5 = 32, B5 = 33, C5 = 34, D5 = 35, E5 = 36, F5 = 37, G5 = 38, H5 = 39,
    A4 = 24, B4 = 25, C4 = 26, D4 = 27, E4 = 28, F4 = 29, G4 = 30, H4 = 31,
    A3 = 16, B3 = 17, C3 = 18, D3 = 19, E3 = 20, F3 = 21, G3 = 22, H3 = 23,
    A2 = 8,  B2 = 9,  C2 = 10, D2 = 11, E2 = 12, F2 = 13, G2 = 14, H2 = 15,
    A1 = 0,  B1 = 1,  C1 = 2,  D1 = 3,  E1 = 4,  F1 = 5,  G1 = 6,  H1 = 7,
}

impl Square {
    pub fn from_string(string: &[u8]) -> Self {
        assert_eq!(
            string.len(),
            2,
            "String must have size == 2 to be converted to Square"
        );

        let file = string[0].to_ascii_lowercase() - b'a';
        let rank = string[1] - b'1';
        let discriminant = rank * 8 + file;
        Square::from_u8(discriminant).expect("Error while converting string into Square")
    }

    pub fn create_unsafe(index: u8) -> Self {
        Square::from_u8(index).unwrap()
    }

    pub fn down_rank(self, ranks: u8) -> Self {
        Square::create_unsafe(self.discriminant() - ranks * 8)
    }

    pub fn discriminant(self) -> u8 {
        self as u8
    }
    
    pub fn flip(self) -> Self {
        let discriminant = self.discriminant();
        let file = discriminant % 8;
        let rank = discriminant / 8;
        let flipped_rank = 7 - rank;
        let flipped_discriminant = flipped_rank * 8 + file;
        Self::create_unsafe(flipped_discriminant)
    }
    
    pub fn rank(&self) -> u8 {
        let rank = self.discriminant() / 8;
        rank
    }
}

#[derive(Debug, Eq, PartialEq, Copy, Clone)]
pub enum MoveType {
    PawnQuiet,
    PawnCapture,
    EnPassant,
    PawnQuietPromotion,
    PawnCapturePromotion,
    KingSideCastle,
    QueenSideCastle,
    RookQuiet,
    KnightQuiet,
    BishopQuiet,
    QueenQuiet,
    KingQuiet,
    RookCapture,
    KnightCapture,
    BishopCapture,
    QueenCapture,
    KingCapture,
}

impl MoveType {
    pub fn is_capture(self) -> bool {
        self == PawnCapture
            || self == KnightCapture
            || self == BishopCapture
            || self == QueenCapture
            || self == KingCapture
            || self == RookCapture
            || self == EnPassant
    }
    
    pub fn is_promotion(self) -> bool {
        self == PawnQuietPromotion || self == PawnCapturePromotion
    }
}

#[derive(Eq, PartialEq, Copy, Clone, Debug)]
pub struct Move {
    pub source: Square,
    pub dst: Square,
    pub move_type: MoveType,
}

impl Move {
    pub fn new(source: Square, dst: Square, move_type: MoveType) -> Self {
        Self {
            source,
            dst,
            move_type,
        }
    }

    pub fn from_string(source: &str, dst: &str) -> Self {
        Self {
            source: Square::from_string(source.as_bytes()),
            dst: Square::from_string(dst.as_bytes()),
            move_type: PawnQuiet,  // no matter what we pass there
        }
    }
    
    pub fn flip(&self) -> Self {
        Self {
            source: self.source.flip(),
            dst: self.dst.flip(),
            move_type: self.move_type,
        }
    }
}

pub enum Player {
    White,
    Black,
}

pub struct Board {
    pub to_move: Player,
    pub is_active: bool,
    pub internal: InternalBoard,
}

impl Board {
    pub fn new() -> Self {
        Self {
            to_move: Player::White,
            is_active: true,
            internal: InternalBoard::default(),
        }
    }

    pub fn play_move(&mut self, move_to_play: Move) {
        unimplemented!()
    }
}
