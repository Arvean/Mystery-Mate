#pragma once

#include "square.h"
#include "piece.h"
#include <unordered_map>
#include <cstdlib>


class Board {
    public:
        Board();
        virtual ~Board() = default;

        void placePiece(const Position& position, Piece* piece); // Remove?
        Piece* getPiece(const Position& position) const; // Remove?
        void movePiece(const Move& move);

        friend class BoardRules;

    private:
        std::unordered_map<Position, Square> squares_;

        void takePiece_(const Square& square);
};