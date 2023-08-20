#pragma once

#include <vector>
#include <stdexcept>
#include "move.h"
#include "position.h"

enum class Color {
    WHITE,
    BLACK
};

class IPiece {
    public:
        virtual IPiece() {};
        virtual IPiece(Color color) {};
        virtual ~IPiece() = default;

        virtual bool isValidMove(const Move& move) const = 0;
        virtual std::vector<Move> getPossibleMoves(const Position& from) const = 0;
        virtual char getSymbol() const = 0;
        virtual Color getColor() const = 0;
};