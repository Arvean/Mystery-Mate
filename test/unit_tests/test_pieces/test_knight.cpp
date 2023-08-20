#include <gtest/gtest.h>
#include "knight.h"
#include "board.h"

TEST(KnightTest, PossibleMoves) {
    Board board;
    Knight knight(Color::WHITE);
    Position position('d', 4);

    board.placePeice(position, knight);

    std::vector<Move> expectedMoveVector;
    expectedMoveVector.push_back(Move(knight, Position('b', 3)));
    expectedMoveVector.push_back(Move(knight, Position('b', 5)));
    expectedMoveVector.push_back(Move(knight, Position('c', 2)));
    expectedMoveVector.push_back(Move(knight, Position('c', 6)));
    expectedMoveVector.push_back(Move(knight, Position('e'. 2)));
    expectedMoveVector.push_back(Move(knight, Position('e', 6)));
    expectedMoveVector.push_back(Move(knight, Position('f', 3)));
    expectedMoveVector.push_back(Move(knight, Position('f', 5)));

    std:vector<Move> possibleMoves = knight.getPossibleMoves();

    for (auto move : expectedMoveVector) {
        EXPECT_TRUE(possibleMoves.find(move) != possibleMoves.end());
    }
}

TEST(KnightTest, LShapedMovement) {
    Board board;
    Knight knight(Color::WHITE);
    Position position('d', 4);

    board.placePeice(position, knight);

    Position to('e', 6);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('f', 5);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('f', 3);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('e', 2);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('c', 2);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('b', 3);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('b', 5);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('c', 6);
    Move move(knight, to);
    EXPECT_TRUE(knight.isValidMove(move));

    Position to('e', 4);
    Move move(knight, to);
    EXPECT_FALSE(knight.isValidMove(move)); // right horizantal

    Position to('c', 4);
    Move move(knight, to);
    EXPECT_FALSE(knight.isValidMove(move)); // right horizantal

    Position to('d', 3);
    Move move(knight, to);
    EXPECT_FALSE(knight.isValidMove(move)); // right horizantal

    Position to('d', 8);
    Move move(knight, to);
    EXPECT_FALSE(knight.isValidMove(move)); // right horizantal
};