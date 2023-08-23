#include "game.h"
#include "board.h"
#include "board_rules.h"

/*
void Game::movePiece(const Move& move, const Board& board) {
    const Square* pSquare = board.getSquare(move.getTo());

    if (pSquare == nullptr) {
        throw std::logic_error("Invalid move. Not a valid square");
    }

    if (board.isValidMove(move)) {
        if (pSquare->isOccupied()) {
            board.takePiece(pSquare->getPiece());
        }
        pSquare->placePiece(move.getPiece());
    } else {
        throw std::logic_error("Invalid move");
    }
};
*/