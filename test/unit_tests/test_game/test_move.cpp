#include <gtest/gtest.h>
#include "pawn.h"
#include "board.h"

TEST(MoveTest, MakeMove) {
    Board board;

    Position from('d', 4);
    Pawn pawn(from, Color::WHITE);
    Position to('d', 5);

    Move move(&pawn, from, to);

    EXPECT_EQ(&pawn, move.getPiece());
    EXPECT_EQ(to, move.getTo());
};