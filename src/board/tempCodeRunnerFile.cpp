#include "board.h"

#define GRID_SIZE 8

Board::Board() {
    for(int rank = 0; rank < GRID_SIZE; ++rank) {
        for(int file = 0; file < GRID_SIZE; ++file) {
            squares_.insert(Square(Position(rank, file)));
        }
    }
};


void Board::placePiece(const Position& position, Piece* piece) {
    auto it = squares_.find(Square(position));
    if (it != squares_.end()) {
        Square& square = const_cast<Square&>(*it);
        square.placePiece(piece);
    } else {
        throw std::logic_error("Invalid square position");
    }
};


Piece* Board::getPiece(const Position& position) const {
    auto it = squares_.find(Square(position));
    if (it != squares_.end()) {
        return it->getPiece();
    } else {
        return nullptr;
    }
};


bool Board::isValidMove(const Move& move) const {

};


void Board::movePiece(const Move& move) {


};


void Board::takePiece_(const Piece& piece) {


};


void Board::promotion_(const Piece& piece) {


};