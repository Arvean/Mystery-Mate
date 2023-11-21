#include "square.h"

bool Square::isOccupied() const {
    return pPiece_ != nullptr;
}


void Square::placePiece(const IPiece* pPiece) {
    if (isOccupied()) {
        throw std::logic_error("Square already occupied");
    }
    pPiece_ = pPiece;
}


const IPiece* Square::getPiece() const {return pPiece_;}


const Position& Square::getPosition() const {return position_;}


void Square::removePiece() {
    pPiece_ = nullptr;
}




