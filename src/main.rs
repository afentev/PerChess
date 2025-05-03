use crate::board::{Board, Move, MoveType, Player, Square};
use crate::engine::Engine;
use crate::internal_board::InternalBoard;
use std::io;
use crate::bitboard::Bitboard;

mod bitboard;
mod board;
mod constants;
mod engine;
mod internal_board;
mod sliding_attacks;

fn get_user_move(is_white: bool) -> Move {
    let mut input_string = String::new();
    io::stdin().read_line(&mut input_string).unwrap();
    let mut parts = input_string.split(" ").map(|s| s.trim());
    let first = parts.next().expect("Empty string entered");
    let second = parts.next().expect("No destination after whitespace");
    let mut res = Move::from_string(first, second);
    if !is_white {
        res = res.flip();
    }
    res
}

fn main() {
    // let board = InternalBoard::default();
    // let moves = board.get_moves();
    // let mut x: Vec<Move> = vec![];
    //
    // println!("{}", x.capacity());
    // x.push(Move::new(Square::A1, Square::A1, MoveType::PawnQuiet));
    // println!("{}", x.capacity());
    // x.push(Move::new(Square::A1, Square::A1, MoveType::PawnQuiet));
    // x.push(Move::new(Square::A1, Square::A1, MoveType::PawnQuiet));
    // x.push(Move::new(Square::A1, Square::A1, MoveType::PawnQuiet));
    // x.push(Move::new(Square::A1, Square::A1, MoveType::PawnQuiet));
    // println!("{}", x.capacity());
    let mut board = Board::new();
    let mut engine = Engine::new(&mut board.internal);
    // println!("{}", Bitboard::from_u64(0b100000000000000000000).flip_board().to_u64());
    // let x = engine.get_engine_move();
    let engine_white = true;
    for i in 0..1 {
        let is_white = i % 2 == 0; 
        if is_white ^ engine_white {
            let move_to_play = get_user_move(is_white);
            engine.receive_move(move_to_play);
        } else {
            engine.get_engine_move(is_white, i);
        }
    }
}
