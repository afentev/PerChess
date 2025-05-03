use crate::bitboard::Bitboard;
use crate::board::{Move, MoveType, Square};
use crate::constants::king_attacks::KING_ATTACKS;
use crate::constants::knight_attacks::KNIGHT_ATTACKS;
use crate::constants::pawn_attacks::PAWN_ATTACKS;
use crate::sliding_attacks::slider_attacks::SliderAttacks;
use std::sync::LazyLock;
use crate::constants::positions::{EG_BISHOP_TABLE, EG_KING_TABLE, EG_KNIGHT_TABLE, EG_PAWN_TABLE, EG_QUEEN_TABLE, EG_ROOK_TABLE, MG_BISHOP_TABLE, MG_KING_TABLE, MG_KNIGHT_TABLE, MG_PAWN_TABLE, MG_QUEEN_TABLE, MG_ROOK_TABLE};

pub(crate) enum MateResult {
    Mate,
    EnemyMate,
    NoMate,
}

#[derive(Copy, Clone)]
pub struct InternalBoard {
    pawns: Bitboard,
    knights: Bitboard,
    rooks: Bitboard,
    bishops: Bitboard,
    queens: Bitboard,
    king: Bitboard,
    occupied: Bitboard,

    enemy_pawns: Bitboard,
    enemy_knights: Bitboard,
    enemy_rooks: Bitboard,
    enemy_bishops: Bitboard,
    enemy_queens: Bitboard,
    enemy_king: Bitboard,
    enemy_occupied: Bitboard,

    total_occupied: Bitboard,

    kingside_castle_allowed: bool,
    queenside_castle_allowed: bool,
    enemy_kingside_castle_allowed: bool,
    enemy_queenside_castle_allowed: bool,
}

impl InternalBoard {
    pub fn default() -> Self {
        let pawns = Bitboard::from_u64(0b1111111100000000);
        let rooks = Bitboard::from_u64(0b10000001);
        let knights = Bitboard::from_u64(0b01000010);
        let bishops = Bitboard::from_u64(0b00100100);
        let queens = Bitboard::from_u64(0b00001000);
        let king = Bitboard::from_u64(0b00010000);
        let occupied = Bitboard::from_u64(0b1111111111111111);
        Self {
            pawns: pawns,
            knights: knights,
            rooks: rooks,
            bishops: bishops,
            queens: queens,
            king: king,
            occupied: occupied,

            enemy_pawns: pawns.flip_board(),
            enemy_knights: knights.flip_board(),
            enemy_rooks: rooks.flip_board(),
            enemy_bishops: bishops.flip_board(),
            enemy_queens: queens.flip_board(),
            enemy_king: king.flip_board(),
            enemy_occupied: occupied.flip_board(),

            total_occupied: Bitboard::from_u64(occupied.to_u64() | occupied.flip_board().to_u64()),

            kingside_castle_allowed: true,
            queenside_castle_allowed: true,
            enemy_kingside_castle_allowed: true,
            enemy_queenside_castle_allowed: true,
        }
    }

    pub fn play_move(&self, move_to_play: Move) -> InternalBoard {
        self.apply_move(move_to_play).flip_board()
    }

    pub fn get_all_moves(&self) -> Vec<Move> {
        let mut moves = Vec::with_capacity(64);
        self.pawn_quiet_moves(&mut moves);
        self.pawn_captures(&mut moves);
        self.knight_moves(&mut moves);
        self.bishop_moves(&mut moves);
        self.rook_moves(&mut moves);
        self.queen_moves(&mut moves);
        self.king_moves(&mut moves);
        moves
    }
    
    pub fn get_captures(&self) -> Vec<Move> {
        let mut moves = Vec::with_capacity(16);
        self.pawn_captures(&mut moves);
        moves
    }

    pub fn get_other_moves(&self) -> Vec<Move> {
        let mut moves = Vec::with_capacity(64);
        self.pawn_quiet_moves(&mut moves);
        self.knight_moves(&mut moves);
        self.bishop_moves(&mut moves);
        self.rook_moves(&mut moves);
        self.queen_moves(&mut moves);
        self.king_moves(&mut moves);
        moves
    }
    
    pub fn mate_status(&self) -> MateResult {
        if self.king.entries_count() == 0 {
            MateResult::Mate
        } else if self.enemy_king.entries_count() == 0 {
            MateResult::EnemyMate
        } else {
            MateResult::NoMate
        }
    }
    
    fn calc_position_reward(piece: MoveType, stage: i32, square: usize) -> i32 {
        match piece {
            MoveType::PawnQuiet => {
                if stage == 0 {
                    MG_PAWN_TABLE[square as usize]
                } else {
                    EG_PAWN_TABLE[square as usize]
                }
            }
            MoveType::RookQuiet => {
                if stage == 0 {
                    MG_ROOK_TABLE[square as usize]
                } else {
                    EG_ROOK_TABLE[square as usize]
                }
            }
            MoveType::KnightQuiet => {
                if stage == 0 {
                    MG_KNIGHT_TABLE[square as usize]
                } else {
                    EG_KNIGHT_TABLE[square as usize]
                }
            }
            MoveType::BishopQuiet => {
                if stage == 0 {
                    MG_BISHOP_TABLE[square as usize]
                } else {
                    EG_BISHOP_TABLE[square as usize]
                }
            }
            MoveType::QueenQuiet => {
                if stage == 0 {
                    MG_QUEEN_TABLE[square as usize]
                } else {
                    EG_QUEEN_TABLE[square as usize]
                }
            }
            MoveType::KingQuiet => {
                if stage == 0 {
                    MG_KING_TABLE[square as usize]
                } else {
                    EG_KING_TABLE[square as usize]
                }
            }
            _ => {
                println!("wtf?");
                0
            }
        }
    }
    
    fn calc(board: Bitboard, piece: MoveType, stage: i32) -> i32 {
        let mut board = board.to_u64();
        let mut sum = 0;
        while board != 0 {
            let idx = board.trailing_zeros() as usize;
            sum += Self::calc_position_reward(piece, stage, idx);
            board &= board - 1; // Clear the least significant set bit
        }
        sum
    }
    
    pub fn measure(&self) -> i32 {
        let kingValue = 20000;
        let queenValue = 1025;
        let rookValue = 477;
        let bishopValue = 365;
        let knightValue = 337;
        let pawnValue = 82;
        let mut score = kingValue * self.king.entries_count() as i32 + queenValue * self.queens.entries_count() as i32 + rookValue * self.rooks.entries_count() as i32 + bishopValue * self.bishops.entries_count() as i32 + knightValue * self.knights.entries_count() as i32 + pawnValue * self.pawns.entries_count() as i32;
        let mut op_score = kingValue * self.enemy_king.entries_count() as i32 + queenValue * self.enemy_queens.entries_count() as i32 + rookValue * self.enemy_rooks.entries_count() as i32 + bishopValue * self.enemy_bishops.entries_count() as i32 + knightValue * self.enemy_knights.entries_count() as i32 + pawnValue * self.enemy_pawns.entries_count() as i32;

        let total = score + op_score;
        let game_stage: i32 = (total < (kingValue * 2 + 2600 * 2)) as i32;
        score += Self::calc(self.pawns, MoveType::PawnQuiet, game_stage);
        score += Self::calc(self.rooks, MoveType::RookQuiet, game_stage);
        score += Self::calc(self.knights, MoveType::KnightQuiet, game_stage);
        score += Self::calc(self.bishops, MoveType::BishopQuiet, game_stage);
        score += Self::calc(self.queens, MoveType::QueenQuiet, game_stage);
        score += Self::calc(self.king, MoveType::KingQuiet, game_stage);
        
        op_score += Self::calc(self.enemy_pawns.flip_board(), MoveType::PawnQuiet, game_stage);
        op_score += Self::calc(self.enemy_rooks.flip_board(), MoveType::RookQuiet, game_stage);
        op_score += Self::calc(self.enemy_knights.flip_board(), MoveType::KnightQuiet, game_stage);
        op_score += Self::calc(self.enemy_bishops.flip_board(), MoveType::BishopQuiet, game_stage);
        op_score += Self::calc(self.enemy_queens.flip_board(), MoveType::QueenQuiet, game_stage);
        op_score += Self::calc(self.enemy_king.flip_board(), MoveType::KingQuiet, game_stage);
        
        score - op_score
    }

    fn pawn_quiet_moves(&self, moves: &mut Vec<Move>) {
        let mut pawns_up = self.pawns.up_one_rank().filter_from(&self.total_occupied);
        let mut pawns_double_up = pawns_up
            .filter_on_third_rank()
            .up_one_rank()
            .filter_from(&self.total_occupied);
        while let Some(pawn_dst) = pawns_up.get_first_entry() {
            let move_type = if pawn_dst.rank() == 8 {
                MoveType::PawnQuietPromotion
            } else {
                MoveType::PawnQuiet
            };
            moves.push(Move::new(
                pawn_dst.down_rank(1),
                pawn_dst,
                move_type,
            ))
        }
        while let Some(pawn_dst) = pawns_double_up.get_first_entry() {
            moves.push(Move::new(
                pawn_dst.down_rank(2),
                pawn_dst,
                MoveType::PawnQuiet,
            ))
        }
    }

    fn pawn_captures(&self, moves: &mut Vec<Move>) {
        let mut pawns = self.pawns;
        while let Some(pawn_pos) = pawns.get_first_entry() {
            let attack_squares = PAWN_ATTACKS[pawn_pos.discriminant() as usize];
            let mut attacks = attack_squares.filter_to(&self.enemy_occupied);
            while let Some(pawn_dst) = attacks.get_first_entry() {
                let move_type = if pawn_dst.rank() == 8 {
                    MoveType::PawnCapturePromotion
                } else {
                    MoveType::PawnCapture
                };
                moves.push(Move::new(pawn_pos, pawn_dst, move_type))
            }
            // todo en-passant croissant ???
        }
    }

    fn knight_moves(&self, moves: &mut Vec<Move>) {
        let mut knights = self.knights;
        while let Some(knight_pos) = knights.get_first_entry() {
            let all_squares = KNIGHT_ATTACKS[knight_pos.discriminant() as usize];
            self.add_moves(
                knight_pos,
                all_squares,
                MoveType::KnightQuiet,
                MoveType::KnightCapture,
                moves,
            );
        }
    }

    fn king_moves(&self, moves: &mut Vec<Move>) {
        let mut king = self.king;
        if let Some(king_pos) = king.get_first_entry() {
            let all_squares = KING_ATTACKS[king_pos.discriminant() as usize];
            self.add_moves(
                king_pos,
                all_squares,
                MoveType::KingQuiet,
                MoveType::KingCapture,
                moves,
            );

            if self.kingside_castle_possible() {
                moves.push(Move::new(king_pos, Square::G1, MoveType::KingSideCastle))
            }
            if self.queenside_castle_possible() {
                moves.push(Move::new(king_pos, Square::C1, MoveType::QueenSideCastle))
            }
        }
    }

    fn bishop_moves(&self, moves: &mut Vec<Move>) {
        let mut bishops = self.bishops;
        while let Some(bishop_pos) = bishops.get_first_entry() {
            let all_squares = SLIDER_ATTACKS.bishop_attacks(self.total_occupied, bishop_pos);
            self.add_moves(
                bishop_pos,
                all_squares,
                MoveType::BishopQuiet,
                MoveType::BishopCapture,
                moves,
            );
        }
    }

    fn rook_moves(&self, moves: &mut Vec<Move>) {
        let mut rooks = self.rooks;
        while let Some(rook_pos) = rooks.get_first_entry() {
            let all_squares = SLIDER_ATTACKS.rook_attacks(self.total_occupied, rook_pos);
            self.add_moves(
                rook_pos,
                all_squares,
                MoveType::RookQuiet,
                MoveType::RookCapture,
                moves,
            );
        }
    }

    fn queen_moves(&self, moves: &mut Vec<Move>) {
        let mut queens = self.queens;
        while let Some(queen_pos) = queens.get_first_entry() {
            let all_squares = SLIDER_ATTACKS.queen_attacks(self.total_occupied, queen_pos);
            self.add_moves(
                queen_pos,
                all_squares,
                MoveType::QueenQuiet,
                MoveType::QueenCapture,
                moves,
            );
        }
    }

    fn kingside_castle_possible(&self) -> bool {
        self.kingside_castle_allowed
            && self.king.is_occupied(Square::E1)
            && self.rooks.is_occupied(Square::H1)
            && !self.total_occupied.is_occupied(Square::F1)
            && !self.total_occupied.is_occupied(Square::G1)
    }

    fn queenside_castle_possible(&self) -> bool {
        self.queenside_castle_allowed
            && self.king.is_occupied(Square::E1)
            && self.rooks.is_occupied(Square::A1)
            && !self.total_occupied.is_occupied(Square::B1)
            && !self.total_occupied.is_occupied(Square::C1)
            && !self.total_occupied.is_occupied(Square::D1)
    }

    fn add_moves(
        &self,
        start: Square,
        all_squares: Bitboard,
        quiet_type: MoveType,
        capture_type: MoveType,
        moves: &mut Vec<Move>,
    ) {
        let squares = all_squares.filter_from(&self.occupied);
        let mut quiet = squares.filter_from(&self.enemy_occupied);
        let mut captures = squares.filter_to(&self.enemy_occupied);
        while let Some(dst) = quiet.get_first_entry() {
            moves.push(Move::new(start, dst, quiet_type))
        }
        while let Some(dst) = captures.get_first_entry() {
            moves.push(Move::new(start, dst, capture_type))
        }
    }

    fn apply_move(&self, move_to_play: Move) -> InternalBoard {
        let mut copy = *self;
        copy.occupied = copy.occupied.make_move(&move_to_play);
        copy.total_occupied = copy.total_occupied.make_move(&move_to_play);

        match move_to_play.move_type {
            MoveType::PawnQuiet => {
                copy.pawns = copy.pawns.make_move(&move_to_play);
            }
            MoveType::PawnCapture => {
                copy.pawns = copy.pawns.make_move(&move_to_play);
                copy.clear_enemy_square(move_to_play.dst);

                copy.pawns = copy.pawns.clear_square(move_to_play.dst);
                copy.atomic_capture(move_to_play.dst);
            }
            MoveType::EnPassant => {
                copy.pawns = copy.pawns.make_move(&move_to_play);
                copy.enemy_pawns.clear_square(move_to_play.dst.down_rank(1));
                copy.enemy_occupied.clear_square(move_to_play.dst);
            }
            MoveType::PawnQuietPromotion => {
                copy.pawns = copy.pawns.clear_square(move_to_play.source);
                copy.queens = copy.queens.occupy_square(move_to_play.dst);
            }
            MoveType::PawnCapturePromotion => {
                copy.pawns = copy.pawns.clear_square(move_to_play.source);
                copy.clear_enemy_square(move_to_play.dst);

                copy.atomic_capture(move_to_play.dst);
            }
            MoveType::KingSideCastle => {
                copy.king = copy.king.make_move(&move_to_play);

                let rook_move = Move {
                    source: Square::H1,
                    dst: Square::F1,
                    move_type: MoveType::KingSideCastle,
                };
                copy.rooks = copy.rooks.make_move(&rook_move);
                copy.occupied = copy.occupied.make_move(&rook_move);
                copy.total_occupied = copy.total_occupied.make_move(&rook_move);

                copy.kingside_castle_allowed = false;
                copy.queenside_castle_allowed = false;
            }
            MoveType::QueenSideCastle => {
                copy.king = copy.king.make_move(&move_to_play);

                let rook_move = Move {
                    source: Square::A1,
                    dst: Square::D1,
                    move_type: MoveType::QueenSideCastle,
                };
                copy.rooks = copy.rooks.make_move(&rook_move);
                copy.occupied = copy.occupied.make_move(&rook_move);
                copy.total_occupied = copy.total_occupied.make_move(&rook_move);

                copy.kingside_castle_allowed = false;
                copy.queenside_castle_allowed = false;
            }
            MoveType::RookQuiet => {
                copy.rooks = copy.rooks.make_move(&move_to_play);
                copy.kingside_castle_allowed &= move_to_play.source != Square::H1;
                copy.queenside_castle_allowed &= move_to_play.source != Square::A1;
            }
            MoveType::KnightQuiet => {
                copy.knights = copy.knights.make_move(&move_to_play);
            }
            MoveType::BishopQuiet => {
                copy.bishops = copy.bishops.make_move(&move_to_play);
            }
            MoveType::QueenQuiet => {
                copy.queens = copy.queens.make_move(&move_to_play);
            }
            MoveType::KingQuiet => {
                copy.king = copy.king.make_move(&move_to_play);
                copy.kingside_castle_allowed = false;
                copy.queenside_castle_allowed = false;
            }
            MoveType::RookCapture => {
                copy.rooks = copy.rooks.make_move(&move_to_play);
                copy.clear_enemy_square(move_to_play.dst);
                copy.kingside_castle_allowed &= move_to_play.source != Square::H1;
                copy.queenside_castle_allowed &= move_to_play.source != Square::A1;

                copy.rooks = copy.rooks.clear_square(move_to_play.dst);
                copy.atomic_capture(move_to_play.dst);
            }
            MoveType::KnightCapture => {
                copy.knights = copy.knights.make_move(&move_to_play);
                copy.clear_enemy_square(move_to_play.dst);

                copy.knights = copy.knights.clear_square(move_to_play.dst);
                copy.atomic_capture(move_to_play.dst);
            }
            MoveType::BishopCapture => {
                copy.bishops = copy.bishops.make_move(&move_to_play);
                copy.clear_enemy_square(move_to_play.dst);

                copy.bishops = copy.bishops.clear_square(move_to_play.dst);
                copy.atomic_capture(move_to_play.dst);
            }
            MoveType::QueenCapture => {
                copy.queens = copy.queens.make_move(&move_to_play);
                copy.clear_enemy_square(move_to_play.dst);

                copy.queens = copy.queens.clear_square(move_to_play.dst);
                copy.atomic_capture(move_to_play.dst);
            }
            MoveType::KingCapture => {
                copy.king = copy.king.make_move(&move_to_play);
                copy.clear_enemy_square(move_to_play.dst);
                copy.kingside_castle_allowed = false;
                copy.queenside_castle_allowed = false;

                copy.king = copy.king.clear_square(move_to_play.dst);
                copy.atomic_capture(move_to_play.dst);
            }
        }

        copy
    }
    
    fn atomic_capture(&mut self, square: Square) {
        let mask = KING_ATTACKS[square.discriminant() as usize];
        self.enemy_king = self.enemy_king.filter_from(&mask);
        self.enemy_queens = self.enemy_queens.filter_from(&mask);
        self.enemy_rooks = self.enemy_rooks.filter_from(&mask);
        self.enemy_bishops = self.enemy_bishops.filter_from(&mask);
        self.enemy_knights = self.enemy_knights.filter_from(&mask);
        self.enemy_occupied = Bitboard::from_u64(self.enemy_occupied.filter_from(&mask).to_u64() | self.enemy_pawns.to_u64());

        self.king = self.king.filter_from(&mask);
        self.queens = self.queens.filter_from(&mask);
        self.rooks = self.rooks.filter_from(&mask);
        self.bishops = self.bishops.filter_from(&mask);
        self.knights = self.knights.filter_from(&mask);
        self.occupied = Bitboard::from_u64(self.occupied.clear_square(square).filter_from(&mask).to_u64() | self.pawns.to_u64());
        self.total_occupied = Bitboard::from_u64(self.occupied.to_u64() | self.enemy_occupied.to_u64());
    }

    fn clear_enemy_square(&mut self, square: Square) {
        self.enemy_pawns = self.enemy_pawns.clear_square(square);
        self.enemy_knights = self.enemy_knights.clear_square(square);
        self.enemy_rooks = self.enemy_rooks.clear_square(square);
        self.enemy_bishops = self.enemy_bishops.clear_square(square);
        self.enemy_queens = self.enemy_queens.clear_square(square);
        self.enemy_king = self.enemy_king.clear_square(square);
        self.enemy_occupied = self.enemy_occupied.clear_square(square);
    }

    fn flip_board(&self) -> InternalBoard {
        InternalBoard {
            pawns: self.enemy_pawns.flip_board(),
            knights: self.enemy_knights.flip_board(),
            rooks: self.enemy_rooks.flip_board(),
            bishops: self.enemy_bishops.flip_board(),
            queens: self.enemy_queens.flip_board(),
            king: self.enemy_king.flip_board(),
            occupied: self.enemy_occupied.flip_board(),
            enemy_pawns: self.pawns.flip_board(),
            enemy_knights: self.knights.flip_board(),
            enemy_rooks: self.rooks.flip_board(),
            enemy_bishops: self.bishops.flip_board(),
            enemy_queens: self.queens.flip_board(),
            enemy_king: self.king.flip_board(),
            enemy_occupied: self.occupied.flip_board(),
            total_occupied: self.total_occupied.flip_board(),
            kingside_castle_allowed: self.enemy_kingside_castle_allowed,
            queenside_castle_allowed: self.enemy_queenside_castle_allowed,
            enemy_kingside_castle_allowed: self.kingside_castle_allowed,
            enemy_queenside_castle_allowed: self.queenside_castle_allowed,
        }
    }

    pub fn enrich_and_play_move(&self, move_to_play: Move) -> InternalBoard {
        let src = move_to_play.source;
        let dst = move_to_play.dst;
        let move_type: MoveType = if self.pawns.is_occupied(src) {
            if self.enemy_occupied.is_occupied(dst) {
                if dst.rank() == 8 {
                    MoveType::PawnCapturePromotion
                } else {
                    MoveType::PawnCapture   
                }
            } else {
                if dst.rank() == 8 {
                    MoveType::PawnQuietPromotion
                } else {
                    MoveType::PawnQuiet   
                }
            }
        } else if self.knights.is_occupied(src) {
            if self.enemy_occupied.is_occupied(dst) {
                MoveType::KnightCapture
            } else {
                MoveType::KnightQuiet
            }
        } else if self.rooks.is_occupied(src) {
            if self.enemy_occupied.is_occupied(dst) {
                MoveType::RookCapture
            } else {
                MoveType::RookQuiet
            }
        } else if self.bishops.is_occupied(src) {
            if self.enemy_occupied.is_occupied(dst) {
                MoveType::BishopCapture
            } else {
                MoveType::BishopQuiet
            }
        } else if self.queens.is_occupied(src) {
            if self.enemy_occupied.is_occupied(dst) {
                MoveType::QueenCapture
            } else {
                MoveType::QueenQuiet
            }
        } else if self.king.is_occupied(src) {
            if self.enemy_occupied.is_occupied(dst) {
                MoveType::KingCapture
            } else if src == Square::E1 && dst == Square::G1 {
                MoveType::KingSideCastle
            } else if src == Square::E1 && dst == Square::C1 {
                MoveType::QueenSideCastle
            } else {
                MoveType::KingQuiet
            }
        } else {
            panic!("unkown move type");
        };
        self.apply_move(Move {source: src, dst, move_type}).flip_board()
    }
}

static SLIDER_ATTACKS: LazyLock<SliderAttacks> = LazyLock::new(SliderAttacks::initialize);
