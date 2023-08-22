#pragma once

#include "piece.h"

class King : public IPiece {
    public:
        King() {};
        King(Color color);

        bool isValidMove(const Move& move) const;
        std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        
        void setHasMoved() {hasMoved_ = true;};
        bool getHasMoved() const {return hasMoved_;}
        char getSymbol() const {return symbol_;}
        Color getColor() const {return color_;}

    private:
        bool hasMoved_;
        const char symbol_;
        const Color color_;
}