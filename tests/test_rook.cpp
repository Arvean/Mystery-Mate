#include "gtest/gtest.h"
#include "rook.h"

TEST(RookTests, IsValidMove) {
    Rook rook;
    Position from('a', 1);
    Position to1('a', 5);
    Move move1(&rook, from, to1);
    EXPECT_TRUE(rook.isValidMove(move1));

    Position to2('d', 1);
    Move move2(&rook, from, to2);
    EXPECT_TRUE(rook.isValidMove(move2));

    Position to3('d', 5);
    Move move3(&rook, from, to3);
    EXPECT_FALSE(rook.isValidMove(move3)); // Invalid move
}

TEST(RookTests, GetPossiblePositions) {
    Rook rook;
    Position from('a', 1);
    auto positions = rook.getPossiblePositions(from);

    EXPECT_EQ(positions.size(), 14);
}

TEST(RookTests, GetSymbol) {
    Rook rook;
    EXPECT_EQ(rook.getType(), PieceType::ROOK);
}

TEST(RookTests, GetColor) {
    Rook rook1(0, Color::WHITE);
    EXPECT_EQ(rook1.getColor(), Color::WHITE);

    Rook rook2(0, Color::BLACK);
    EXPECT_EQ(rook2.getColor(), Color::BLACK);
}

TEST(RookTests, HasMoved) {
    Rook rook;
    EXPECT_FALSE(rook.getHasMoved()); // Should be false initially

    rook.setHasMoved();
    EXPECT_TRUE(rook.getHasMoved()); // Should be true after calling setHasMoved
}
