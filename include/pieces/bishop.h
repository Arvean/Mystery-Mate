#pragma once

#include "piece.h"

class Bishop : public IPiece {
    public:
        Bishop() {};
        Bishop(Color color) {color_ = color;}

        bool isValidMove(const Move& move) const;
        std::vector<Move> getPossibleMoves(const Position& from) const;
        char getSymbol() const {return symbol_;}
        Color getColor() const {return color_;}

    private:
        const char symbol_;
        const Color color_;
}