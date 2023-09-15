#include "pawn.h"
#include "gtest/gtest.h"

TEST(PawnTests, IsValidMove) {
    Pawn pawnWhite(0, Color::WHITE);
    Position from('d', 4);

    // Pawn's valid move (one step forward)
    Move validMoveOneStep(&pawnWhite, from, Position('d', 5));
    // Invalid move for Pawn (diagonal without capturing)
    Move invalidMoveDiagonal(&pawnWhite, from, Position('d', 6));

    EXPECT_TRUE(pawnWhite.isValidMove(validMoveOneStep));
    EXPECT_FALSE(pawnWhite.isValidMove(invalidMoveDiagonal));
}

TEST(PawnTests, IsValidTwoStepMove) {
    Pawn pawnWhite(0, Color::WHITE);
    Position startingPositionWhite('e', 2);

    // White Pawn's valid two-step move from starting position
    Move validTwoStepMoveWhite(&pawnWhite, startingPositionWhite, Position('e', 4));

    // Black Pawn's valid two-step move from starting position
    Pawn pawnBlack(0, Color::BLACK);
    Position startingPositionBlack('e', 7);
    Move validTwoStepMoveBlack(&pawnWhite, startingPositionBlack, Position('e', 5));

    // Invalid two-step move (not from starting position)
    Move invalidTwoStepMove(&pawnWhite, Position('e', 4), Position('e', 6));

    EXPECT_TRUE(pawnWhite.isValidMove(validTwoStepMoveWhite));
    EXPECT_TRUE(pawnBlack.isValidMove(validTwoStepMoveBlack));
    EXPECT_FALSE(pawnWhite.isValidMove(invalidTwoStepMove));
}

TEST(PawnTests, GetPossiblePositions) {
    Pawn pawnWhite(0, Color::WHITE);
    Position from('d', 2);

    auto positions = pawnWhite.getPossiblePositions(from);

    EXPECT_EQ(positions.size(), 2);
}

TEST(PawnTests, GetSymbol) {
    Pawn pawnWhite(0, Color::WHITE);

    EXPECT_EQ(pawnWhite.getType(), PieceType::PAWN);
}

TEST(PawnTests, GetColor) {
    Pawn pawnWhite(0, Color::WHITE);

    EXPECT_EQ(pawnWhite.getColor(), Color::WHITE);

    Pawn pawnBlack(0, Color::BLACK);

    EXPECT_EQ(pawnBlack.getColor(), Color::BLACK);
}