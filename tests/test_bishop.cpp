#include "bishop.h"
#include "gtest/gtest.h"

TEST(BishopTests, GetPossiblePositions) {
    Bishop bishopWhite(0, Color::WHITE);
    Position from('d', 4);
    
    auto positions = bishopWhite.getPossiblePositions(from);
    
    EXPECT_EQ(positions.size(), 13);
}

TEST(BishopTests, IsValidMove) {
    Bishop bishopWhite(0, Color::WHITE);
    Position from('d', 4);
    
    Move validMove(&bishopWhite, from, Position('f', 6));
    Move invalidMove(&bishopWhite, from, Position('d', 6));
    
    EXPECT_TRUE(bishopWhite.isValidMove(validMove));
    EXPECT_FALSE(bishopWhite.isValidMove(invalidMove));
}

TEST(BishopTests, GetSymbol) {
    Bishop bishopWhite(0, Color::WHITE);
    
    EXPECT_EQ(bishopWhite.getType(), PieceType::BISHOP);
}

TEST(BishopTests, GetColor) {
    Bishop bishopWhite(0, Color::WHITE);
    
    EXPECT_EQ(bishopWhite.getColor(), Color::WHITE);

    Bishop bishopBlack(0, Color::BLACK);

    EXPECT_EQ(bishopBlack.getColor(), Color::BLACK);
}
