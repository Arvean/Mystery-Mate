#include <gtest/gtest.h>
#include "pawn.h"
#include "bishop.h"
#include "board.h"

TEST(BoardTest, SetPiece) {
    Board board;
    Position from('d', 4);

    board.placePiece(from, Pawn(Color::WHITE));
    Piece* pPawn = board.getPiece(position);

    EXPECT_EQ(pPawn, &pawn);
};

TEST(BoardTest, ValidMove) {
    Board board;
    Position from('d', 4);

    Pawn pawn(Color::WHITE);
    board.placePiece(from, pawn);

    Move move(pawn, from, Position('d', 5));

    EXPECT_TRUE(board.isValidMove(move));
};

TEST(BoardTest, ObstructedMove) {
    Board board;
    Position from('d', 4);

    Bishop bishop(Color::WHITE);
    board.placePiece(from, bishop);


    Pawn pawn(Color::BLACK);
    board.placePiece(Position('f', 6), pawn);

    Move move(bishop, from, Position('g', 7));

    EXPECT_FALSE(board.isValidMove(move));
};

TEST(BoardTest, MoveToBusySquare) {
    Board board;

    Position from('d', 4);
    Position to('e', 5);

    Pawn whitePawn(Color::WHITE);
    board.placePiece(from, Pawn(Color::WHITE));

    Pawn blackPawn(Color::BLACK);
    board.placePiece(to, Pawn(Color::BLACK));

    Move move(whitePawn, from, to); //initiate takePiece_
    
    Piece* pPawn = board.getPiece(to);

    EXPECT_EQ(pPawn, &whitePawn);
}