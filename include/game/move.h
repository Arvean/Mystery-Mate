#pragma once

#include "piece.h"
#include "position.h"

class Move {
    // Container to hold the chosen move
    public:
        Move() {};
        Move(const Piece& piece, const Position& to);
        virtual ~Move() = default;

        const Piece& getPiece() const;
        const Position& getTo() const;

    private:
        Piece piece_;
        Position to_;
}
/*
        } else if (dynamic_cast<Pawn*>(move.getPiece()) != nullptr) {
            if (isPromoted_()) {
                promotion_()
                move.getTo().getFile() == GRID_SIZE
            }
        }
*/