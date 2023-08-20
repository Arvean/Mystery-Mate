#include <gtest/gtest.h>
#include "piece.h"
#include "pawn.h"
#include "board.h"

TEST(BoardTest, SetPiece) {
    Board board;

    board.placePiece(Position(d, 4), Pawn(Color::WHITE));
    Piece* pPawn = board.getPiece(position);

    EXPECT_EQ(pPawn, &pawn);
};

TEST(BoardTest, ValidMove) {
    Board board;

    Pawn pawn(Color::WHITE);
    board.placePiece(Position(d, 4), pawn);

    Move move(pawn, Position(d, 5));

    EXPECT_TRUE(board.isValidMove(move));
};

TEST(BoardTest, MoveToBusySquare) {
    Board board;

    Position to(e, 5);

    Pawn whitePawn(Color::WHITE);
    board.placePiece(Position(d, 4), Pawn(Color::WHITE));

    Pawn blackPawn(Color::BLACK);
    board.placePiece(to, Pawn(Color::BLACK));

    Move move(whitePawn, to); //initiate takePiece_
    
    Piece* pPawn = board.getPiece(to);

    EXPECT_EQ(pPawn, &whitePawn);
}

TEST(BoardTest, EnPassant) {
    Board board;

    Pawn whitePawn(Color::WHITE);
    Position whiteFrom(d, 2);
    Position whiteTo(d, 4);
    board.placePiece(whiteFrom, Pawn(Color::WHITE));

    Pawn blackPawn(Color::BLACK);
    Position blackFrom(d, 3);
    board.placePiece(blackFrom, Pawn(Color::BLACK));

    Move move(whitePawn, whiteTo); //initiate takePiece_ (EnPassant)
    
    Piece* pPawn = board.getPiece(blackFrom);

    EXPECT_EQ(pPawn, &whitePawn);
}