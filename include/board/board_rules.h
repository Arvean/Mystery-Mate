#pragma once

#include "Board.h"
#include "Move.h"
#include "Position.h"

class BoardRules {
    public:
        BoardRules(Board& b) : board_(b) {}

        bool isValidMove(const Move& move) const;
        bool isInCheck(Color color) const;
        bool isValidCastling(const Move& kingMove, const Move& rookMove) const;
        bool isValidEnPassant(const Move& move, const Position& capturedPawnPosition) const;

    private:
        bool isObstructed_(Position& position);
        bool isObstructedBetweenRank_(const Position& from, const Position& to);
        bool isObstructedBetweenFile_(const Position& from, const Position& to);
        std::unordered_set<Position> getAttackedPositions_(Color color) const;
        Position& findKing_(Color color) const;
        
        Board& board_;
}