# What is PerChess?
PerChess is a chess engine written in C++. It uses MTD(f) algorithm to find the best moves. It usually wins a 2100-rated engine on chess.com.

<p align="center">
  <img src="https://github.com/afentev/PerChess/blob/main/Screen%20Shot%202021-05-21%20at%2010.59.49%20PM.png">
</p>

# How does it work?
PerChess is based on the Sunfish engine. It also uses MTD(f) algorithm. PerChess supports databases for openings in endings. 

Now it uses a small opening database and I have plans to make this database much bigger. The ending database is a Sygyzy table for 3-, 4- and 5-pieces endings. 

MTD(f) algorithm in this implementation is looking for moves in simple heuristic order; in fact, it is very similar to MVV-LVA. The evaluation function thanks to Sunfish is very quick. It uses two types of piece-square tables: one for opening and middlegame and the other for the endgame.

The searching function is upper limited with 20 seconds for penultimate depth so, in fact, it takes about 30 seconds to make a turn.

# How strong is it?
I made several games between PerChess and the 2100-rated engine on chess.com. PerChess played them all as black. It never lost and won several games. It generally wins against the 2000-rated engine. It also seems like it is weaker than the 2200-rated engine so it should be about 2100 rated bot. 

# What is this app?
This whole app is a simple chess program. It has UI and you can try to play against one of 3 bots. 

The simplest bot always makes random moves. Two other bots use the same PerChess algorithm but the Standart bot has a 0.01-second upper limit and the Pro bot has 20 seconds.

# Dependencies and borrowings
First of all this program is written in C++. It uses Qt 5 for UI. PerChess engine use [Lichess elite database](https://database.nikonoel.fr) for openings, [Sygyzy tables](https://syzygy-tables.info) for endings, [Fathom library](https://github.com/basil00/Fathom) to work with these tables, [THC chess library](https://github.com/billforsternz/thc-chess-library) for table representation and connectivity with opening and endgame databases, and of course it is based on the [Sunfish](https://github.com/thomasahle/sunfish) chess engine.
