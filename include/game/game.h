#pragma once
#include "board.h"
#include "move.h"

class Game {
public:
    Game();
    virtual ~Game() = default;

    bool isValidMove(const Move& move) const;
    void makeMove(const Move& move);
    bool isInCheck(Color color) const;
    bool isValidCastling(const Move& kingMove, const Move& rookMove) const;
    bool isValidEnPassant(const Move& move, const Position& capturedPawnPosition) const;

private:
    Board board_;
    std::vector<MoveRecord> moveHistory_; // assuming MoveRecord is a class/struct to track move details

    // You might include other private helper functions or state as needed
};