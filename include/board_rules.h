#pragma once

#include "piece.h"
#include "king.h"
#include "player.h"
#include "board.h"
#include <unordered_set>
#include <unordered_map>

class BoardRules {
    public:
        BoardRules() {};
        virtual ~BoardRules() = default;

        virtual bool isValidMove(const Board& board, const Move& move, const Move& previousMove);
        virtual bool isInCheck(const Board& board, const Color kingColor) const;
        virtual bool isValidCastling(const Board& board, const Move& kingMove) const;
        virtual bool isValidEnPassant(const Move& previousMove, const Move& move) const;
        virtual bool isValidPromotion(const Move& move) const;
        
        virtual std::unordered_set<Position> generateValidPositions(const Board& board, const IPiece* piece, const Position& from, const Move& previousMove);

    private:
        void _availablePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& position, const Move& previousMove);
        void _addKingCastlingPositions(const Board& board, std::unordered_set<Position>& possiblePositions, const King* king, const Position& from) const;
        void _addPawnCapturePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from, const Move& previousMove) const;
        void _removeKingInCheckPositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from); 
};