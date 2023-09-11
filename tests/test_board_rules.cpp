#include <gtest/gtest.h>
#include "board.h"
#include "board_rules.h"
#include "mock_piece.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "pawn.h"

// Test if a move is valid
TEST(BoardRules, IsValidMove_Valid) {
    Board board;
    BoardRules rules;
    // You would typically set up a specific state here, e.g., place pieces on the board
    Position from{'e', 2};
    Position to{'e', 4};

    Queen queen;
    Queen* pQueen = &queen;
    board.placePiece(from, pQueen);

    Move move(pQueen, from, to); // Set up a valid move for the given piece and positions
    EXPECT_TRUE(rules.isValidMove(board, move));
}

// Test if a move is not valid
TEST(BoardRules, IsValidMove_NotValid) {
    Board board;
    BoardRules rules;
    // You would typically set up a specific state here, e.g., place pieces on the board
    Position from{'e', 2};
    Position to{'e', 4};

    Queen queen;
    Queen* pQueen = &queen;
    board.placePiece(from, pQueen);

    Pawn pawn;
    Pawn* pPawn = &pawn;
    board.placePiece(Position('e', 3), pPawn); 

    Move move(pQueen, from, to); // Set up a valid move for the given piece and positions
    EXPECT_FALSE(rules.isValidMove(board, move));
}

// Test if a king is in check
TEST(BoardRules, IsInCheck) {
    Board board;
    BoardRules rules;

    King king = King(0, Color::BLACK);
    King* pKing = &king;

    Queen queen = Queen();
    Queen* pQueen = &queen;

    board.placePiece(Position('e', 4), pQueen);
    board.placePiece(Position('e', 8), pKing);
    // Set up specific scenario where the king is in check
    EXPECT_TRUE(rules.isInCheck(board, pKing->getColor()));
}

// Test if a castling move is valid
TEST(BoardRules, IsValidCastling) {
    Board board;
    BoardRules rules;

    King king = King(0, Color::BLACK);
    King* pKing = &king;

    Rook rook = Rook(0, Color::BLACK);
    Rook* pRook = &rook;

    Position kfrom('e', 1);
    Position kto('g', 1);

    Position rfrom('h', 1);
    Position rto('f', 1);

    board.placePiece(kfrom, pKing);
    board.placePiece(rfrom, pRook);
    // Set up specific scenario for castling
    Move kingMove(pKing, kfrom, kto);
    Move rookMove(pRook, rfrom, rto);

    EXPECT_TRUE(rules.isValidCastling(board, kingMove, rookMove));
}

// Test if an en passant move is valid
TEST(BoardRules, IsValidEnPassant) {
    Board board;
    BoardRules rules;

    Pawn whitePawn(0, Color::WHITE);
    Pawn blackPawn(0, Color::BLACK);

    Move previousMove(&whitePawn, Position('d', 2), Position('d', 4));
    Move move(&blackPawn, Position('c', 3), Position('d', 2));

    EXPECT_TRUE(rules.isValidEnPassant(previousMove, move));
}

// Test if a promotion move is valid
TEST(BoardRules, IsValidPromotion) {
    Board board;
    BoardRules rules;

    Pawn pawn = Pawn();
    Pawn* pPawn = &pawn;

    Position from('d', 7);
    Position to ('d', 8);

    board.placePiece(from, pPawn);
    Move move(pPawn, from, to);

    Queen newQueen = Queen();
    Queen* pNewQueen = &newQueen;

    EXPECT_TRUE(rules.isValidPromotion(move, pNewQueen));
}
