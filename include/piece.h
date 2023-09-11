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

enum class PieceType {
    MOCK,
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};

class IPiece {
    public:
        IPiece() {};
        IPiece(int id, Color color) {};
        virtual ~IPiece() = default;

        virtual IPiece* clone() const = 0; //For the piece copy constructor

        virtual bool isValidMove(const Move& move) const = 0;

        virtual std::unordered_set<Position> getPossiblePositions(const Position& from) const = 0;
        virtual PieceType getType() const = 0;
        virtual Color getColor() const = 0;
        virtual int getID() const = 0;

        static char fileToChar(int fileInt) {return 'a' + fileInt;}
        static int charToFile(char fileChar) {return fileChar - 'a';}
};