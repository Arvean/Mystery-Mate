#include <gtest/gtest.h>
#include "pawn.h"
#include "board.h"

TEST(PawnTest, PossibleMoves) {
    Board board;
    Pawn pawn(Color::WHITE);
    Position position('d', 4);

    board.placePeice(position, pawn);

    std::vector<Move> expectedMoveVector;

    // Vertical
    expectedMoveVector.push_back(Move(pawn, Position('d', 4)));
    expectedMoveVector.push_back(Move(pawn, Position('d', 3)));
    
    std:vector<Move> possibleMoves = pawn.getPossibleMoves();

    for (auto move : expectedMoveVector) {
        EXPECT_TRUE(possibleMoves.find(move) != possibleMoves.end());
    }
}

TEST(PawnTest, VerticalMovement) {
    Board board;
    Pawn pawn(Color::WHITE);
    Position position('d', 2);

    board.placePeice(position, pawn);

    Position to('d', 4);
    Move move(pawn, to);
    EXPECT_TRUE(pawn.isValidMove(move)); // up one square

    Position to('d', 3);
    Move move(pawn, to);
    EXPECT_TRUE(pawn.isValidMove(move)); // up two squares

    Position to('e', 4);
    Move move(pawn, to);
    EXPECT_FALSE(pawn.isValidMove(move)); // right horizantal

    Position to('c', 4);
    Move move(pawn, to);
    EXPECT_FALSE(pawn.isValidMove(move)); // right horizantal

    Position to('d', 8);
    Move move(pawn, to);
    EXPECT_FALSE(pawn.isValidMove(move)); // right horizantal

    Position to('e', 5);
    Move move(pawn, to);
    EXPECT_FALSE(pawn.isValidMove(move)); // right horizantal
};