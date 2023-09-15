#include "knight.h"
#include "gtest/gtest.h"

TEST(KnightTests, IsValidMove) {
    Knight knightWhite(0, Color::WHITE);
    Position from('d', 4);

    // Knight's valid move (L-shape)
    Move validMove(&knightWhite, from, Position('f', 5));
    // Invalid move for Knight
    Move invalidMove(&knightWhite, from, Position('d', 6));

    EXPECT_TRUE(knightWhite.isValidMove(validMove));
    EXPECT_FALSE(knightWhite.isValidMove(invalidMove));
}

TEST(KnightTests, GetPossiblePositions) {
    Knight knightWhite(0, Color::WHITE);
    Position from('d', 4);

    auto positions = knightWhite.getPossiblePositions(from);

    EXPECT_EQ(positions.size(), 8);
}

TEST(KnightTests, GetSymbol) {
    Knight knightWhite(0, Color::WHITE);

    EXPECT_EQ(knightWhite.getType(), PieceType::KNIGHT);
}

TEST(KnightTests, GetColor) {
    Knight knightWhite(0, Color::WHITE);

    EXPECT_EQ(knightWhite.getColor(), Color::WHITE);

    Knight knightBlack(0, Color::BLACK);

    EXPECT_EQ(knightBlack.getColor(), Color::BLACK);
}
