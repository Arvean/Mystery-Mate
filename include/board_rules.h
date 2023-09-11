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

        bool isValidMove(const Board& board, const Move& move) const;
        bool isInCheck(const Board& board, const Color kingColor) const;
        bool isValidCastling(const Board& board, const Move& kingMove, const Move& rookMove) const;
        bool isValidEnPassant(const Move& previousMove, const Move& move) const;
        bool isValidPromotion(const Move& move, IPiece* newPiece) const;

    private:
        void _addPawnCapturePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from) const;
        void _removeKingInCheckMoves(const Board& board, std::unordered_set<Position>& possiblePositions, const Move& move) const; 
};