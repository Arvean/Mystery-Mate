#include <gtest/gtest.h>
#include "bishop.h"
#include "board.h"

TEST(BishopTest, PossibleMoves) {
    Board board;
    Bishop bishop(Color::WHITE);
    Position position('d', 4);

    board.placePeice(position, bishop);

    std::vector<Move> expectedMoveVector;
    expectedMoveVector.push_back(Move(bishop, Position('c', 3)));
    expectedMoveVector.push_back(Move(bishop, Position('f', 6)));
    expectedMoveVector.push_back(Move(bishop, Position('g', 7)));
    expectedMoveVector.push_back(Move(bishop, Position('h', 8)));
    expectedMoveVector.push_back(Move(bishop, Position('b', 2)));
    expectedMoveVector.push_back(Move(bishop, Position('a', 1)));
    expectedMoveVector.push_back(Move(bishop, Position('e', 3)));
    expectedMoveVector.push_back(Move(bishop, Position('f', 2)));
    expectedMoveVector.push_back(Move(bishop, Position('g', 1)));
    expectedMoveVector.push_back(Move(bishop, Position('c', 5)));
    expectedMoveVector.push_back(Move(bishop, Position('b', 6)));
    expectedMoveVector.push_back(Move(bishop, Position('a', 7)));

    std:vector<Move> possibleMoves = bishop.getPossibleMoves();

    for (auto move : expectedMoveVector) {
        EXPECT_TRUE(possibleMoves.find(move) != possibleMoves.end());
    }
}

TEST(BishopTest, DiagnolMovement) {
    Board board;
    Bishop bishop(Color::WHITE);
    Position position('d', 4);

    board.placePeice(position, bishop);

    Position to('h', 8);
    Move move(bishop, to);
    EXPECT_TRUE(bishop.isValidMove(move)); // diagnol right

    Position to('a', 1);
    Move move(to)
    EXPECT_TRUE(bishop.isValidMove(move)); // diagnol left

    Position to('e', 4);
    Move move(bishop, to);
    EXPECT_FALSE(bishop.isValidMove(move)); // right horizantal

    Position to('c', 4);
    Move move(bishop, to);
    EXPECT_FALSE(bishop.isValidMove(move)); // right horizantal

    Position to('d', 3);
    Move move(bishop, to);
    EXPECT_FALSE(bishop.isValidMove(move)); // right horizantal

    Position to('d', 5);
    Move move(bishop, to);
    EXPECT_FALSE(bishop.isValidMove(move)); // right horizantal
};