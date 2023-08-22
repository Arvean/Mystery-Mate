#include <gtest/gtest.h>
#include "pawn.h"
#include "bishop.h"
#include "board.h"

TEST(BoardTest, EnPassant) {
    Board board;

    Pawn whitePawn(Color::WHITE);
    Position whiteFrom('d', 2);
    Position whiteTo('d', 4);
    board.placePiece(whiteFrom, Pawn(Color::WHITE));

    Pawn blackPawn(Color::BLACK);
    Position blackFrom('d', 3);
    board.placePiece(blackFrom, Pawn(Color::BLACK));

    Move move(whitePawn, whiteFrom, whiteTo); //initiate takePiece_ (EnPassant)
    
    Piece* pPawn = board.getPiece(blackFrom);

    EXPECT_EQ(pPawn, &whitePawn);
}