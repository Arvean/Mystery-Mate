#include "gtest/gtest.h"
#include "king.h"

TEST(KingTests, IsValidMove) {
    King king;
    Position from('e', 1);
    Position to1('e', 2);
    Move move1(&king, from, to1);
    EXPECT_TRUE(king.isValidMove(move1));

    Position to2('d', 1);
    Move move2(&king, from, to2);
    EXPECT_TRUE(king.isValidMove(move2));

    Position to3('d', 3);
    Move move3(&king, from, to3);
    EXPECT_FALSE(king.isValidMove(move3)); // Invalid move
}

TEST(KingTests, GetPossiblePositions) {
    King king;
    Position from('e', 1);
    auto positions = king.getPossiblePositions(from);

    EXPECT_EQ(positions.size(), 8);
}

TEST(KingTests, GetSymbol) {
    King king;
    EXPECT_EQ(king.getSymbol(), 'K');
}

TEST(KingTests, GetColor) {
    King king1(Color::WHITE);
    EXPECT_EQ(king1.getColor(), Color::WHITE);

    King king2(Color::BLACK);
    EXPECT_EQ(king2.getColor(), Color::BLACK);
}

TEST(KingTests, HasMoved) {
    King king;
    EXPECT_FALSE(king.getHasMoved()); // Should be false initially

    king.setHasMoved();
    EXPECT_TRUE(king.getHasMoved()); // Should be true after calling setHasMoved
};