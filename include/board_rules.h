#pragma once

#include "piece.h"
#include "player.h"
#include "board.h"
#include <unordered_set>
#include <unordered_map>

class BoardRules {
    public:
        BoardRules() {};
        virtual ~BoardRules() = default;

        virtual bool isValidMove(const Board& board, const Move& move, const Move& previousMove = Move()) const;
        virtual bool isInCheck(const Board& board, const Color kingColor) const;
        virtual bool isValidCastling(const Board& board, const Move& kingMove) const;
        virtual bool isValidEnPassant(const Move& previousMove, const Move& move) const;
        virtual bool isValidPromotion(const Move& move) const;

    private:
        void _addPawnCapturePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from) const;
        void _removeKingInCheckMoves(const Board& board, std::unordered_set<Position>& possiblePositions, const Move& move) const; 
};