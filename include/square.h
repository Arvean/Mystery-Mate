#pragma once

#include "piece.h"

class Square {
    public:
        Square() {};
        Square(Position position)
            : position_(position) {};
        ~Square() = default;

        virtual bool isOccupied() const;
        virtual const Position& getPosition() const;
        
        virtual void placePiece(const IPiece* pPiece);
        virtual const IPiece* getPiece() const;
        virtual void removePiece();

    private:
        const Position position_;
        const IPiece* pPiece_ = nullptr;
};