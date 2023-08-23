#pragma once

#include "square.h"
#include "piece.h"
#include <unordered_map>
#include <cstdlib>
#include <memory>


class Board {
    public:
        Board();
        virtual ~Board() = default;

        void placePiece(const Position& position, IPiece* piece); // Remove?
        const Square* getSquare(const Position& position) const; // Remove?
        void takePiece(Square& square);

        friend class BoardRules;

    private:
        std::unordered_map<Position, std::unique_ptr<Square>> squares_;

        bool isObstructed_(const Position& position) const;
        bool isObstructedBetweenRank_(const Position& from, const Position& to) const;
        bool isObstructedBetweenFile_(const Position& from, const Position& to) const;
        
        std::unordered_set<Position> getAttackedPositions_(Color color) const;
        Position findKing_(Color color) const;
};