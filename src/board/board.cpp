#include "board.h"

#define GRID_SIZE 8

Board::Board() {
    for(int rank = 0; rank < GRID_SIZE; ++rank) {
        for(char file = 'a'; file <= 'h'; ++file) {
            squares_[Position(rank, file)] = Square(Position(rank, file));
        }
    }
};


void Board::placePiece(const Position& position, Piece* piece) {
    auto it = squares_.find(position);
    if (it != squares_.end()) {
        it->second.placePiece(piece);
    } else {
        throw std::logic_error("Invalid square position");
    }
};


Piece* Board::getPiece(const Position& position) const {
    auto it = squares_.find(position);
    if (it != squares_.end()) {
        return it->second.getPiece();
    } else {
        return nullptr;
    }
};


bool Board::isValidMove(const Move& move) const {

};


void Board::movePiece(const Move& move) {
    auto it = squares_.find(move.getTo());

    if (it == squares_.end()) {
        throw std::logic_error("Invalid move. Not a valid square");
    }

    Square& square = it->second;

    if (isValidMove(move)) {
        if (square.isOccupied()) {
            takePiece_(square.getPiece());
        }
        square.placePiece(move.getPiece());
    } else {
        throw std::logic_error("Invalid move");
    }
};


void Board::takePiece_(const Square& square) {
    if (square.isOccupied()) {
        square.removePiece();
    } else {
        std::logic_error("Invalid. No piece to remove.");
    }
};

/*
bool Board::checkPromotion_(Piece* piece, Position& position) {
    if (dynamic_cast<Pawn*>(piece != nullptr) {
        if (piece.getColor() == Color::WHITE) {
            return (position.getRank() == GRID_SIZE);
        } 
        else if (piece.getColor() == Color::BLACK) {
            return (position.getRank() == 1);
        }
    }
    return false;
}
/*