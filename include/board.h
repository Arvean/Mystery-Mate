#pragma once

#include "square.h"
#include "piece.h"
#include <unordered_map>
#include <cstdlib>
#include <memory>


class Board {
    public:
        Board();
        Board(const Board& other);
        virtual ~Board() = default;

        friend class BoardRules;

        virtual Board& operator=(const Board& other);

        virtual void placePiece(const Position& position, IPiece* piece);
        virtual void removePiece(Square* pSquare);
        virtual Square* getSquare(const Position& position) const;

        static bool isLightSquare(const Position& pos);
        static bool isDarkSquare(const Position& pos);

        std::unordered_map<Position, std::unique_ptr<Square>> squares;

    private:

        bool isObstructed_(const Position& position) const;
        bool isObstructedBetweenRank_(const Position& from, const Position& to) const;
        bool isObstructedBetweenFile_(const Position& from, const Position& to) const;
        
        bool isInsideBoard_(const Position& position) const;
        std::unordered_set<Position> getAttackedPositions_(Color color) const;
        Position findKing_(Color color) const;
};