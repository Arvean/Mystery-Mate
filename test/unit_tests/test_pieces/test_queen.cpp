#include <gtest/gtest.h>
#include "queen.h"
#include "board.h"

TEST(QueenTest, PossibleMoves) {
    Board board;
    Queen queen(Color::WHITE);
    Position position('d', 4);

    board.placePeice(position, queen);

    std::vector<Move> expectedMoveVector;
    // Diagnol
    expectedMoveVector.push_back(Move(queen, Position('b', 3)));
    expectedMoveVector.push_back(Move(queen, Position('b', 5)));
    expectedMoveVector.push_back(Move(queen, Position('c', 2)));
    expectedMoveVector.push_back(Move(queen, Position('c', 6)));
    expectedMoveVector.push_back(Move(queen, Position('e', 2)));
    expectedMoveVector.push_back(Move(queen, Position('e', 6)));
    expectedMoveVector.push_back(Move(queen, Position('f', 3)));
    expectedMoveVector.push_back(Move(queen, Position('f', 5)));

    // Vertical
    expectedMoveVector.push_back(Move(queen, Position('d', 1)));
    expectedMoveVector.push_back(Move(queen, Position('d', 2)));
    expectedMoveVector.push_back(Move(queen, Position('d', 3)));
    expectedMoveVector.push_back(Move(queen, Position('d', 5)));
    expectedMoveVector.push_back(Move(queen, Position('d', 6)));
    expectedMoveVector.push_back(Move(queen, Position('d', 7)));
    expectedMoveVector.push_back(Move(queen, Position('d', 8)));

    // Horizantal
    expectedMoveVector.push_back(Move(queen, Position('a', 4)));
    expectedMoveVector.push_back(Move(queen, Position('b', 4)));
    expectedMoveVector.push_back(Move(queen, Position('c', 4)));
    expectedMoveVector.push_back(Move(queen, Position('e', 4)));
    expectedMoveVector.push_back(Move(queen, Position('f', 4)));
    expectedMoveVector.push_back(Move(queen, Position('g', 4)));
    expectedMoveVector.push_back(Move(queen, Position('h', 4)));
    

    std:vector<Move> possibleMoves = queen.getPossibleMoves();

    for (auto move : expectedMoveVector) {
        EXPECT_TRUE(possibleMoves.find(move) != possibleMoves.end());
    }
}

TEST(QueenTest, HorizantalMovement) {
    Board board;
    Position queen('d', 4);
    Queen queen(Color::WHITE);

    Position to('h', 4);
    Move move(queen, to);
    EXPECT_TRUE(queen.isValidMove(move)); // right

    Position to('a', 4);
    Move move(queen, to);
    EXPECT_TRUE(queen.isValidMove(move)); // left

    Position to('h', 3);
    Move move(queen, to);
    EXPECT_FALSE(queen.isValidMove(move));
};

TEST(QueenTest, VerticalMovement) {
    Board board;
    Queen queen('d', 4, Color::WHITE);

    Position to('d', 8);
    Move move(queen, to);
    EXPECT_TRUE(queen.isValidMove(move)); // up

    Position to('d', 1);
    Move move(queen, to);
    EXPECT_TRUE(queen.isValidMove(move)); // down

    Position to('c', 1);
    Move move(queen, to);
    EXPECT_FALSE(queen.isValidMove(move));
};

TEST(QueenTest, DiagnolMovement) {
    Board board;
    Position queen('d', 4);
    Queen queen(queen, Color::WHITE);

    Position to('h', 8);
    Move move(queen, to);
    EXPECT_TRUE(queen.isValidMove(move)); // diagnol right

    Position to('a', 1);
    Move move(queen, to);
    EXPECT_TRUE(queen.isValidMove(move)); // diagnol left

    Position to('g', 8);
    Move move(queen, to);
    EXPECT_FALSE(queen.isValidMove(move));
};