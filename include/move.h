#pragma once

#include "position.h"

class IPiece;

class Move {
    // Container to hold the chosen move
    public:
        Move() 
            : pPiece_(nullptr), from_(Position(0, 0)), to_(Position(0, 0)) {};
        Move(const IPiece* const pPiece, const Position& from, const Position& to) 
            : pPiece_(pPiece), from_(from), to_(to) {};
        virtual ~Move() = default;

        const IPiece* getPiece() const {return pPiece_;}
        const Position& getFrom() const {return from_;}
        const Position& getTo() const {return to_;}

    private:
        const IPiece* pPiece_;
        const Position from_;
        const Position to_;
};