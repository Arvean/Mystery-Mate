#include <gtest/gtest.h>
#include "square.h"
#include "mock_piece.h"

// Test the default constructor
TEST(Square, DefaultConstructor) {
    Square square;
    // Here, you might want to test any default behavior or state
}

// Test the constructor with a position
TEST(Square, ConstructorWithPosition) {
    Position pos{'e', 4};
    Square square(pos);
    EXPECT_EQ(square.getPosition(), pos);
}

// Test the isOccupied method
TEST(Square, IsOccupied) {
    Position pos{'e', 4};
    MockPiece piece;
    Square square(pos);
    EXPECT_FALSE(square.isOccupied());
    square.placePiece(&piece);
    EXPECT_TRUE(square.isOccupied());
}

// Test the placePiece method
TEST(Square, PlacePiece) {
    Position pos{'e', 4};
    MockPiece piece;
    Square square(pos);
    square.placePiece(&piece);
    EXPECT_EQ(square.getPiece(), &piece);
}

// Test the getPiece method
TEST(Square, GetPiece) {
    Position pos{'e', 4};
    MockPiece piece;
    Square square(pos);
    EXPECT_EQ(square.getPiece(), nullptr);
    square.placePiece(&piece);
    EXPECT_EQ(square.getPiece(), &piece);
}

// Test the getPosition method
TEST(Square, GetPosition) {
    Position pos{'e', 4};
    Square square(pos);
    EXPECT_EQ(square.getPosition(), pos);
}

// Test the removePiece method
TEST(Square, RemovePiece) {
    Position pos{'e', 4};
    MockPiece piece;
    Square square(pos);
    square.placePiece(&piece);
    square.removePiece();
    EXPECT_EQ(square.getPiece(), nullptr);
    EXPECT_FALSE(square.isOccupied());
}

// Test placing a piece when the square is already occupied
TEST(Square, PlacePieceWhenOccupied) {
    Position pos{'e', 4};
    MockPiece piece1, piece2;
    Square square(pos);
    square.placePiece(&piece1);
    square.placePiece(&piece2); // The behavior here may vary based on your implementation
    // Here, you may want to check if the square contains the first or second piece based on how you've implemented placePiece
    // For example, if placing a second piece on an occupied square replaces the existing piece:
    EXPECT_EQ(square.getPiece(), &piece2);
}
