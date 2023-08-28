#pragma once

#include "piece.h"

class Square {
    public:
        Square() {};
        Square(Position position)
            : position_(position) {};
        ~Square() = default;

        bool const isOccupied() const;
        const Position& getPosition() const;
        
        void placePiece(const IPiece* pPiece);
        const IPiece* getPiece() const;
        void removePiece();

    private:
        const Position position_;
        const IPiece* pPiece_ = nullptr;
};