Tic-Tac-Toe AI

A high-performance Tic-Tac-Toe engine written in C++ that uses Bitboards for state representation and Minimax with Alpha-Beta Pruning 
and a Transposition Table for an AI that always plays the most optimal moves.

Features

Bitboard Representation: Uses 16-bit integers to represent the board, allowing for extremely fast win-checking and move generation using bitwise operations.

Unbeatable AI: Implements the Minimax algorithm. The bot will either win or force a draw—it is mathematically impossible to beat.

Alpha-Beta Pruning: Optimized search that skips branches of the game tree that don't affect the final decision.

Transposition Table (TT): Caches previously evaluated board positions to avoid redundant calculations, significantly speeding up the engine.

Move Randomization: If multiple moves have the same optimal score, the bot chooses one randomly to ensure varied gameplay.

How to Compile

Using g++:

g++ main.cpp -o tictactoe

Run:

./tictactoe

How to Play

When prompted, choose your side: X or O.

Input your move using row and column indices (0, 1, or 2).

Example: 0 0 for the top-left corner.

The AI will instantly calculate the best response and play the move on the board.
