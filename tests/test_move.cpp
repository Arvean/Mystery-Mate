#include "gtest/gtest.h"
#include "move.h"
#include "piece.h"
#include "mock_piece.h"


TEST(MoveTests, DefaultConstructor) {
    Move move;

    EXPECT_EQ(move.getPiece(), nullptr);
    EXPECT_EQ(move.getFrom(), Position(0, 0));
    EXPECT_EQ(move.getTo(), Position(0, 0));
}

TEST(MoveTests, ParameterizedConstructor) {
    MockPiece piece;
    Position from('a', 1);
    Position to('b', 2);
    Move move(&piece, from, to);
    EXPECT_EQ(move.getPiece(), &piece);
    EXPECT_EQ(move.getFrom(), from);
    EXPECT_EQ(move.getTo(), to);
}

TEST(MoveTests, GetPiece) {
    MockPiece piece;
    Move move(&piece, Position('c', 3), Position('d', 4));
    EXPECT_EQ(move.getPiece(), &piece);
}

TEST(MoveTests, GetFrom) {
    Position from('e', 5);
    Move move(nullptr, from, Position('f', 6));
    EXPECT_EQ(move.getFrom(), from);
}

TEST(MoveTests, GetTo) {
    Position to('g', 7);
    Move move(nullptr, Position('h', 8), to);
    EXPECT_EQ(move.getTo(), to);
}
