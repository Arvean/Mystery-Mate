#include "square.h"

Square::Square(Position position) {position_ = position;}


bool const Square::isOccupied() const {
    return pPiece_ != nullptr;
}


void Square::placePiece(Piece* pPiece) {
    if (isOccupied()) {
        throw std::logic_error("Square already occupied");
    }
    pPiece_ = pPiece;
}


Piece* Square::getPiece() const {return pPiece_;}


Piece* Square::getPosition() const {return position_;}


void Square::removePiece() {
    pPiece_ = nullptr;
}


