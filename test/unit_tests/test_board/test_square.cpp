#include <gtest/gtest.h>
#include "square.h"

TEST(SquareTest, PlacePiece) {
    Position pos_d4('d', 4);
    Square d4(pos_d4);

    Piece pawn(Color::BLACK);

    d4.placePiece(&pawn);
    Piece* expectedpPawn = getPiece();

    EXPECT_EQ(&pawn, expectedpPawn);
};

TEST(SquareTest, PlacePieceWhenOccupied) {
    Position pos_d4(d, 4);
    Square d4(pos_d4);

    Piece pawn1(Color::BLACK);
    Piece pawn2(Color::WHITE);

    d4.placePiece(&pawn1);

    EXPECT_THROW(d4.placePiece(&pawn2), std::logic_error);
}