#pragma once
#include "board.h"
#include "move.h"

class Game {
public:
    Game() {};
    virtual ~Game() = default;

    void movePiece(const Move& move, const Board& board);

private:
    Board board_;
    //std::vector<MoveRecord> moveHistory_;

};