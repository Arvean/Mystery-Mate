#pragma once

#include "piece.h"
#include "position.h"

class Move {
    // Container to hold the chosen move
    public:
        Move() {};
        Move::Move(const Piece* pPiece, const Position& from, const Position& to) 
            : pPiece_(pPiece), from_(from), to_(to) {};
        virtual ~Move() = default;

        const Piece* getPiece() const {return pPiece_;}
        const Position& getFrom() const {return from_;}
        const Position& getTo() const {return to_;}

    private:
        Piece* pPiece_;
        Position from_;
        Position to_;
};