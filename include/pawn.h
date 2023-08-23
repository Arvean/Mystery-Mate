#pragma once

#include "piece.h"

class Pawn : public IPiece {
    public:
        Pawn() : symbol_('P'), color_(Color::WHITE) {};
        Pawn(Color color) : symbol_('P'), color_(color) {};
    
        bool isValidMove(const Move& move) const;
        std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        char getSymbol() const {return symbol_;}
        Color getColor() const {return color_;}

    private:
        const char symbol_;
        const Color color_;
};