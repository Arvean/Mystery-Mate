#include "move.h"

Move::Move(const Piece* pPiece, const Position& to) 
    : pPiece_(pPiece), to_(to) {};


const Piece* Move::getPiece() const {return pPiece_};


const Position& Move::getTo() const {return to_};