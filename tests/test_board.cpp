#include <gtest/gtest.h>
#include "board.h"
#include "mock_piece.h"

// Test constructing an empty board
TEST(Board, ConstructEmptyBoard) {
    Board board;
    // You may want to verify some properties of an empty board here,
    // depending on your implementation.
    // For example, you might iterate over all squares and verify that they are all empty.
}

// Test placing a piece on the board
TEST(Board, PlacePiece) {
    Board board;
    Position pos{'e', 4};
    MockPiece piece;
    board.placePiece(pos, &piece);
    EXPECT_EQ(board.getSquare(pos)->getPiece(), &piece);
}

// Test taking a piece from a square
TEST(Board, RemovePiece) {
    Board board;
    Position pos{'e', 4};
    MockPiece piece;
    board.placePiece(pos, &piece);
    board.removePiece(board.getSquare(pos));
    EXPECT_EQ(board.getSquare(pos)->getPiece(), nullptr);
}

// Test getting a square that does not contain a piece
TEST(Board, GetEmptySquare) {
    Board board;
    Position pos{'e', 4};
    EXPECT_EQ(board.getSquare(pos)->getPiece(), nullptr);
}

