#pragma once

#include <unordered_set>
#include <vector>
#include <stdexcept>
#include "move.h"
#include "position.h"

#define GRID_SIZE 8

enum class Color {
    WHITE,
    BLACK
};

class IPiece {
    public:
        IPiece() {};
        IPiece(Color color) {};
        virtual ~IPiece() = default;

        virtual bool isValidMove(const Move& move) const = 0;
        virtual std::unordered_set<Position> getPossiblePositions(const Position& from) const = 0;
        virtual char getSymbol() const = 0;
        virtual Color getColor() const = 0;

    protected:
        char fileToChar_(int fileInt) const {return 'a' + fileInt;}
        int charToFile_(char fileChar) const {return fileChar - 'a';}
};