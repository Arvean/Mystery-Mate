#pragma once

#include "piece.h"
#include "board.h"
#include <unordered_set>
#include <unordered_map>

class BoardRules {
    public:
        BoardRules() {};
        virtual ~BoardRules() = default;

        bool isValidMove(const Board& board, const Move& move) const;
        bool isInCheck(const Board& board, const IPiece* king) const;
        bool isValidCastling(const Board& board, const Move& kingMove, const Move& rookMove) const;
        bool isValidEnPassant(const Move& previousMove, const Move& move) const;
        bool isValidPromotion(const Move& move, IPiece* newPiece) const;

};