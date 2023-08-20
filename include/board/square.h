#pragma once

#include "piece.h"

template<>
struct std::hash<Position> {
    size_t operator()(const Position& position) const {
        return std::hash<int>()(position.getX()) ^ std::hash<int>()(position.getY());
    }
};

class Square {
    public:
        Square() {};
        Square(Position position);
        ~Square() = default;

        bool const isOccupied() const;
        void placePiece(Piece* pPiece);
        Piece* getPiece() const;
        Position& getPosition() const;
        void removePiece();

    private:
        Position position_;
        Piece* pPiece_ = nullptr;
}