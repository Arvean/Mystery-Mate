#include "gtest/gtest.h"
#include "queen.h"

TEST(QueenTests, IsValidHorizontalMove) {
    Queen queen;
    Position from('d', 4);
    Position to('h', 4);
    Move move(&queen, from, to);
    EXPECT_TRUE(queen.isValidMove(move));
}

TEST(QueenTests, IsValidVerticalMove) {
    Queen queen;
    Position from('e', 5);
    Position to('e', 1);
    Move move(&queen, from, to);
    EXPECT_TRUE(queen.isValidMove(move));
}

TEST(QueenTests, IsValidDiagonalMove) {
    Queen queen;
    Position from('a', 1);
    Position to('h', 8);
    Move move(&queen, from, to);
    EXPECT_TRUE(queen.isValidMove(move));
}

TEST(QueenTests, IsInvalidLShapeMove) {
    Queen queen;
    Position from('a', 1);
    Position to('c', 2); // L-shape move, not valid for a queen
    Move move(&queen, from, to);
    EXPECT_FALSE(queen.isValidMove(move));
}

TEST(QueenTests, GetPossiblePositions) {
    Queen queen;
    Position from('d', 4);
    auto positions = queen.getPossiblePositions(from);

    EXPECT_EQ(positions.size(), 27);
}

TEST(QueenTests, GetSymbol) {
    Queen queen;
    EXPECT_EQ(queen.getSymbol(), 'Q');
}

TEST(QueenTests, GetColor) {
    Queen queen1(Color::WHITE);
    EXPECT_EQ(queen1.getColor(), Color::WHITE);

    Queen queen2(Color::BLACK);
    EXPECT_EQ(queen2.getColor(), Color::BLACK);
};




