#include <gtest/gtest.h>
#include "rook.h"
#include "board.h"

TEST(RookTest, PossibleMoves) {
    Board board;
    Rook rook(Color::WHITE);
    Position from('d', 4);

    board.placePeice(from, rook);

    std::vector<Move> expectedMoveVector;

    // Vertical
    expectedMoveVector.push_back(Move(rook, from, Position('d', 1)));
    expectedMoveVector.push_back(Move(rook, from, Position('d', 2)));
    expectedMoveVector.push_back(Move(rook, from, Position('d', 3)));
    expectedMoveVector.push_back(Move(rook, from, Position('d', 5)));
    expectedMoveVector.push_back(Move(rook, from, Position('d', 6)));
    expectedMoveVector.push_back(Move(rook, from, Position('d', 7)));
    expectedMoveVector.push_back(Move(rook, from, Position('d', 8)));

    // Horizantal
    expectedMoveVector.push_back(Move(rook, from, Position('a', 4)));
    expectedMoveVector.push_back(Move(rook, from, Position('b', 4)));
    expectedMoveVector.push_back(Move(rook, from, Position('c', 4)));
    expectedMoveVector.push_back(Move(rook, from, Position('e', 4)));
    expectedMoveVector.push_back(Move(rook, from, Position('f', 4)));
    expectedMoveVector.push_back(Move(rook, from, Position('g', 4)));
    expectedMoveVector.push_back(Move(rook, from, Position('h', 4)));
    

    std:vector<Move> possibleMoves = rook.getPossibleMoves();

    for (auto move : expectedMoveVector) {
        EXPECT_TRUE(possibleMoves.find(move) != possibleMoves.end());
    }
}

TEST(RookTest, HorizantalMovement) {
    Board board;
    Position from('d', 4);
    Rook rook(Color::WHITE);

    Position to('h', 4);
    Move move(rook, from, to);
    EXPECT_TRUE(rook.isValidMove(move)); // right

    Position to('a', 4);
    Move move(rook, from, to);
    EXPECT_TRUE(rook.isValidMove(move)); // left

    Position to('e', 5);
    Move move(rook, from, to);
    EXPECT_FALSE(rook.isValidMove(move)); // diagnol
};

TEST(RookTest, VerticalMovement) {
    Board board;
    Position from('d', 4);
    Rook rook(Color::WHITE);

    Position to('d', 8);
    Move move(rook, from, to);
    EXPECT_TRUE(rook.isValidMove(move)); // up

    Position to('d', 1);
    Move move(rook, from, to);
    EXPECT_TRUE(rook.isValidMove(move)); // down

    Position to('b', 6);
    Move move(rook, from, to);
    EXPECT_FALSE(rook.isValidMove(move)); // diagnol

    Position to('c', 3);
    Move move(rook, from, to);
    EXPECT_FALSE(rook.isValidMove(move)); // diagnol
};