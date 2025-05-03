use crate::bitboard::Bitboard;
use crate::board::Square;
use std::fs;

pub struct SliderAttacks {
    rook_masks: [u64; 64],
    bishop_masks: [u64; 64],
    rook_magics: [u64; 64],
    bishop_magics: [u64; 64],
    rook_offsets: [u64; 64],
    bishop_offsets: [u64; 64],
    rook_shifts: [u64; 64],
    bishop_shifts: [u64; 64],
    rook_attack_table: [u64; 102400],
    bishop_attack_table: [u64; 5248],
}

impl SliderAttacks {
    pub fn initialize() -> Self {
        let rook_masks = Self::read_from_file::<64>("src/sliding_attacks/magics/rook_masks.magic");
        let bishop_masks =
            Self::read_from_file::<64>("src/sliding_attacks/magics/bishop_masks.magic");
        let rook_magics =
            Self::read_from_file::<64>("src/sliding_attacks/magics/rook_magics.magic");
        let bishop_magics =
            Self::read_from_file::<64>("src/sliding_attacks/magics/bishop_magics.magic");
        let rook_offsets =
            Self::read_from_file::<64>("src/sliding_attacks/magics/rook_offsets.magic");
        let bishop_offsets =
            Self::read_from_file::<64>("src/sliding_attacks/magics/bishop_offsets.magic");
        let rook_shifts =
            Self::read_from_file::<64>("src/sliding_attacks/magics/rook_shifts.magic");
        let bishop_shifts =
            Self::read_from_file::<64>("src/sliding_attacks/magics/bishop_shifts.magic");
        let rook_attack_table =
            Self::read_from_file::<102400>("src/sliding_attacks/magics/rook_attack_table.magic");
        let bishop_attack_table =
            Self::read_from_file::<5248>("src/sliding_attacks/magics/bishop_attack_table.magic");
        Self {
            rook_masks,
            bishop_masks,
            rook_magics,
            bishop_magics,
            rook_offsets,
            bishop_offsets,
            rook_shifts,
            bishop_shifts,
            rook_attack_table,
            bishop_attack_table,
        }
    }

    pub fn queen_attacks(&self, bitboard: Bitboard, square: Square) -> Bitboard {
        let index = square.discriminant();
        let rook_attacks = self.rook_attack_table[self.rook_table_index(bitboard, index)];
        let bishop_attacks = self.bishop_attack_table[self.bishop_table_index(bitboard, index)];
        Bitboard::from_u64(rook_attacks | bishop_attacks)
    }

    pub fn rook_attacks(&self, bitboard: Bitboard, square: Square) -> Bitboard {
        let index = square.discriminant();
        Bitboard::from_u64(self.rook_attack_table[self.rook_table_index(bitboard, index)])
    }

    pub fn bishop_attacks(&self, bitboard: Bitboard, square: Square) -> Bitboard {
        let index = square.discriminant();
        Bitboard::from_u64(self.bishop_attack_table[self.bishop_table_index(bitboard, index)])
    }

    fn rook_table_index(&self, bitboard: Bitboard, index: u8) -> usize {
        let mask = self.rook_masks[index as usize];
        let magic = self.rook_magics[index as usize];
        let shift = self.rook_shifts[index as usize];
        let offset = self.rook_offsets[index as usize];

        Self::attack_table_index(bitboard, mask, magic, shift, offset)
    }

    fn bishop_table_index(&self, bitboard: Bitboard, index: u8) -> usize {
        let mask = self.bishop_masks[index as usize];
        let magic = self.bishop_magics[index as usize];
        let shift = self.bishop_shifts[index as usize];
        let offset = self.bishop_offsets[index as usize];

        Self::attack_table_index(bitboard, mask, magic, shift, offset)
    }

    fn attack_table_index(
        bitboard: Bitboard,
        mask: u64,
        magic: u64,
        shift: u64,
        offset: u64,
    ) -> usize {
        let occupancy = bitboard.to_u64() & mask;
        let index = ((occupancy.wrapping_mul(magic)) >> (64 - shift)) + offset;
        index as usize
    }

    fn read_from_file<const N: usize>(filepath: &str) -> [u64; N] {
        let content = fs::read_to_string(filepath).expect("Failed to read file");
        *content
            .split('\n')
            .map(|line| u64::from_str_radix(line, 16).expect("Unable to parse file"))
            .collect::<Vec<u64>>()
            .first_chunk::<N>()
            .unwrap_or_else(|| panic!("File must contain {} lines", N))
    }
}
