#pragma once

#include "piece.h"

class Bishop : public IPiece {
    public:
        Bishop() : symbol_('B'), color_(Color::WHITE) {};
        Bishop(Color color) : symbol_('B'), color_(color) {};

        bool isValidMove(const Move& move) const;
        std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        char getSymbol() const {return symbol_;}
        Color getColor() const {return color_;}

    private:
        const char symbol_;
        const Color color_;
};