use std::cmp::{Ordering, PartialEq};
use std::mem;
use std::ops::Neg;
use crate::board::{Move, MoveType, Square};
use crate::internal_board::{InternalBoard, MateResult};

use circular_buffer::CircularBuffer;
use crate::board;

#[derive(PartialEq, Eq, Copy, Clone, Debug)]
struct Evaluation {
    evaluation: i32,
    force_mate_cnt: Option<usize>,
    storage: Option<usize>,
}

impl PartialOrd<Self> for Evaluation {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Evaluation {
    fn cmp(&self, other: &Self) -> Ordering {
        match (self.force_mate_cnt, other.force_mate_cnt) {
            (Some(self_cnt), Some(other_cnt)) => self_cnt.cmp(&other_cnt),
            (Some(_), None) => Ordering::Less,
            (None, Some(_)) => Ordering::Greater,
            (None, None) => self.evaluation.cmp(&other.evaluation)
        }
    }
}

impl Neg for Evaluation {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Self {
            evaluation: -self.evaluation,
            force_mate_cnt: self.storage,
            storage: self.force_mate_cnt,
        }
    }
}

pub struct Engine<'a> {
    board: &'a mut InternalBoard,
}

impl<'a> Engine<'a> {
    pub fn new(internal_board: &'a mut InternalBoard) -> Self {
        Self {
            board: internal_board,
        }
    }

    pub fn get_engine_move(&mut self, is_white: bool, turn: usize) {
        let alpha = Evaluation {evaluation: -100000, force_mate_cnt: Some(0), storage: None};
        let beta = Evaluation {evaluation: 100000, force_mate_cnt: None, storage: None };
        let depth = (9.0 + turn as f64 * 0.1) as usize;
        println!("depth: {}", depth);
        let mut killers = vec![CircularBuffer::<3, Move>::new(); depth + 1];
        let r = self.nega_max(alpha, beta, depth, 0, &mut killers);
        let mut mv = r.1.unwrap();
        let board = self.board.play_move(mv);
        if !is_white {
            mv = mv.flip();
        }
        println!("{:?} {:?}", r.0, mv);
        let _ = mem::replace(self.board, board);
    }

    fn nega_max(&mut self, mut alpha: Evaluation, beta: Evaluation, depth_left: usize, deep: usize, killers: &mut Vec<CircularBuffer<3, Move>>) -> (Evaluation, Option<Move>) {
        if depth_left == 0 {
            return (Evaluation {evaluation: self.board.measure(), force_mate_cnt: None, storage: None}, None);
        }
        let mut best_value = Evaluation {evaluation: -100000, force_mate_cnt: Some(0), storage: None };
        let mut best_move: Option<Move> = None;

        let captures = self.board.get_captures();
        if let Some(value) = self.traverse_moves(captures, &mut alpha, beta, &mut best_value, &mut best_move, depth_left, deep, killers) {
            return (value, None);
        }
        
        killers[deep + 1].clear();
        if let Some(value) = self.traverse_moves(killers[deep].to_vec(), &mut alpha, beta, &mut best_value, &mut best_move, depth_left, deep, killers) {
            return (value, None);
        }

        let mut other_moves = self.board.get_other_moves();
        other_moves.sort_unstable_by_key(|m| {
            if m.move_type.is_capture() {
                return 1;
            }
            2
        });

        if let Some(value) = self.traverse_moves(other_moves, &mut alpha, beta, &mut best_value, &mut best_move, depth_left, deep, killers) {
            return (value, None);
        }
        (best_value, best_move)
    }

    fn traverse_moves<'b>(&mut self, moves: Vec<Move>, alpha: &mut Evaluation, beta: Evaluation, best_value: &mut Evaluation, best_move: &mut Option<Move>, depth_left : usize, deep : usize, killers : &mut Vec<CircularBuffer<3, Move>>) -> Option<Evaluation>
    {  // some if cutoff happened
        for move_to_make in moves {
            let mut new_board = self.board.play_move(move_to_make);
            let score = match new_board.mate_status() {
                MateResult::Mate => Evaluation {evaluation: 50000, force_mate_cnt: None, storage: None },
                MateResult::EnemyMate => Evaluation {evaluation: -50000, force_mate_cnt: Some(deep), storage: None },
                MateResult::NoMate => {
                    mem::swap(self.board, &mut new_board);
                    let res = -self.nega_max(-beta, -*alpha, depth_left - 1, deep + 1, killers).0;
                    let _ = mem::replace(self.board, new_board);
                    res
                }
            };

            if score > *best_value {
                *best_value = score;
                if score > *alpha {
                    *alpha = score;
                }
                *best_move = Some(move_to_make);
            }
            if score >= beta {
                if !move_to_make.move_type.is_capture() && !move_to_make.move_type.is_promotion() {
                    killers[deep].push_front(move_to_make);
                }
                return Some(*best_value);
            }
        }
        None
    }


    // fn alpha_beta_max(&mut self, mut alpha: Evaluation, beta: Evaluation, depth_left: usize, deep: usize, killers: &mut Vec<CircularBuffer<3, Move>>) -> (Evaluation, Option<Move>) {
    //     if depth_left == 0 {
    //         return (Evaluation {evaluation: self.board.measure(), force_mate_cnt: None, storage: None}, None);
    //     }
    //     killers[deep + 1].clear();
    //     // for killer in killers[deep].clone() {
    //     //     let mut new_board = self.board.play_move(killer);
    //     //     let score = match new_board.mate_status() {
    //     //         MateResult::Mate => Evaluation {evaluation: 50000, force_mate_cnt: None, storage: None },
    //     //         MateResult::EnemyMate => Evaluation {evaluation: -50000, force_mate_cnt: Some(deep), storage: None },
    //     //         MateResult::NoMate => {
    //     //             mem::swap(self.board, &mut new_board);
    //     //             let res = self.alpha_beta_min(alpha, beta, depth_left - 1, deep + 1, killers);
    //     //             let _ = mem::replace(self.board, new_board);
    //     //             res
    //     //         }
    //     //     };
    //     //
    //     //     if score >= beta {
    //     //         return (beta, None);
    //     //     }
    //     //     if score > alpha {
    //     //         alpha = Evaluation {evaluation: score.evaluation, force_mate_cnt: score.force_mate_cnt, storage: None};
    //     //     }
    //     // }
    //     let mut moves = self.board.get_moves();
    //     moves.sort_unstable_by_key(|m| {
    //         if killers[deep].iter().any(|km| km == m) {
    //             return 0;
    //         }
    //         if m.move_type.is_capture() {
    //             return 1;
    //         }
    //         2
    //     });
    //     let mut best_move: Option<Move> = None;
    //     for move_to_make in moves {
    //         let mut new_board = self.board.play_move(move_to_make);
    //         let score = match new_board.mate_status() {
    //             MateResult::Mate => Evaluation {evaluation: 50000, force_mate_cnt: None, storage: None },
    //             MateResult::EnemyMate => Evaluation {evaluation: -50000, force_mate_cnt: Some(deep), storage: None },
    //             MateResult::NoMate => {
    //                 mem::swap(self.board, &mut new_board);
    //                 let res = self.alpha_beta_min(alpha, beta, depth_left - 1, deep + 1, killers);
    //                 let _ = mem::replace(self.board, new_board);
    //                 res
    //             }
    //         };
    //
    //         if score >= beta {
    //             if !move_to_make.move_type.is_capture() && !move_to_make.move_type.is_promotion() {
    //                 killers[deep].push_front(move_to_make);
    //             }
    //             return (beta, None);
    //         }
    //         if score > alpha {
    //             alpha = Evaluation {evaluation: score.evaluation, force_mate_cnt: score.force_mate_cnt, storage: None};
    //             best_move = Some(move_to_make);
    //         }
    //     }
    //     (alpha, best_move)
    // }
    //
    // fn alpha_beta_min(&mut self, alpha: Evaluation, mut beta: Evaluation, depth_left: usize, deep: usize, killers: &mut Vec<CircularBuffer<3, Move>>) -> Evaluation {
    //     if depth_left == 0 {
    //         return Evaluation {evaluation: -self.board.measure(), force_mate_cnt: None, storage: None};
    //     }
    //     killers[deep + 1].clear();
    //     // for killer in killers[deep].clone() {
    //     //     let mut new_board = self.board.play_move(killer);
    //     //
    //     //     let score = match new_board.mate_status() {
    //     //         MateResult::Mate => Evaluation{evaluation: -50000, force_mate_cnt: Some(deep), storage: None },
    //     //         MateResult::EnemyMate => Evaluation{evaluation: 50000, force_mate_cnt: None, storage: None },
    //     //         MateResult::NoMate => {
    //     //             mem::swap(self.board, &mut new_board);
    //     //             let res = self.alpha_beta_max(alpha, beta, depth_left - 1, deep + 1, killers).0; // todo
    //     //             let _ = mem::replace(self.board, new_board);
    //     //             res
    //     //         }
    //     //     };
    //     //
    //     //     if score <= alpha {
    //     //         return alpha;
    //     //     }
    //     //     if score < beta {
    //     //         beta = score;
    //     //     }
    //     // }
    //     let mut moves = self.board.get_moves();
    //     moves.sort_unstable_by_key(|m| {
    //         if killers[deep].iter().any(|km| km == m) {
    //             return 0;
    //         }
    //         if m.move_type.is_capture() {
    //             return 1;
    //         }
    //         2
    //     });
    //     for move_to_make in moves {
    //         let mut new_board = self.board.play_move(move_to_make);
    //
    //         let score = match new_board.mate_status() {
    //             MateResult::Mate => Evaluation{evaluation: -50000, force_mate_cnt: Some(deep), storage: None },
    //             MateResult::EnemyMate => Evaluation{evaluation: 50000, force_mate_cnt: None, storage: None },
    //             MateResult::NoMate => {
    //                 mem::swap(self.board, &mut new_board);
    //                 let res = self.alpha_beta_max(alpha, beta, depth_left - 1, deep + 1, killers).0; // todo
    //                 let _ = mem::replace(self.board, new_board);
    //                 res
    //             }
    //         };
    //
    //         if score <= alpha {
    //             if !move_to_make.move_type.is_capture() && !move_to_make.move_type.is_promotion() {
    //                 killers[deep].push_front(move_to_make);
    //             }
    //             return alpha;
    //         }
    //         if score < beta {
    //             beta = score;
    //         }
    //     }
    //     beta
    // }

    pub fn receive_move(&mut self, move_to_make: Move) {
        let board = self.board.enrich_and_play_move(move_to_make);
        let _ = mem::replace(self.board, board);
    }
}
