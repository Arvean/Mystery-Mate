#pragma once

#include "piece.h"

#define GRID_SIZE 8U

class Pawn : public IPiece {
    public:
        Pawn() {};
        Pawn(Color color);
    
        bool isValidMove(const Move& move) const;
        std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        char getSymbol() const {return symbol_;}
        Color getColor() const {return color_;}

    private:
        const char symbol_;
        const Color color_;
}