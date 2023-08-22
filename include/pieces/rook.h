#pragma once

#include "piece.h"

class Rook : public IPiece {
    public:
        Rook() {};
        Rook(Color color);

        bool isValidMove(const Move& move) const;
        std::unordered_set<Position> getPossiblePositions(const Position& from) const;

        bool hasMoved() const {return hasMoved_;}
        char getSymbol() const {return symbol_;}
        Color getColor() const {return color_;}

    private:
        bool hasMoved_;
        const char symbol_;
        const Color color_;
}