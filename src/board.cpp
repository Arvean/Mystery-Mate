#include "board.h"

Board::Board() {
    for(int rank = 0; rank < GRID_SIZE; ++rank) {
        for(char file = 'a'; file <= 'h'; ++file) {
            squares_[Position(rank, file)] = std::make_unique<Square>(Position(rank, file));
        }
    }
};


void Board::placePiece(const Position& position, IPiece* piece) {
    auto it = squares_.find(position);
    if (it != squares_.end()) {
        it->second->placePiece(piece);
    } else {
        throw std::logic_error("Invalid square position");
    }
};


Square* Board::getSquare(const Position& position) const {
    auto it = squares_.find(position);
    if (it != squares_.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
};

void Board::takePiece(Square& square) {
    if (square.isOccupied()) {
        square.removePiece();
    } else {
        throw std::logic_error("Invalid. No piece to remove.");
    }
};


bool Board::isObstructed_(const Position& position) const {
    const Square* pSquare = getSquare(position);

    return pSquare->getPiece() != nullptr;
};


bool Board::isObstructedBetweenRank_(const Position& from, const Position& to) const {
    int fromRank = from.getRank();
    int toRank = to.getRank();
    char file = from.getFile();

    int step = (fromRank < toRank) ? 1 : -1;

    for (int rank = fromRank + step; rank != toRank; rank += step) {
        auto it = squares_.find(Position(rank, file));
        if (it != squares_.end() && it->second->isOccupied()) {
            return true; // If a square is occupied, then there's an obstruction
        }
    }
    return false;
};


bool Board::isObstructedBetweenFile_(const Position& from, const Position& to) const {
    char fromFile = from.getFile();
    char toFile = to.getFile();
    int rank = from.getRank();

    int step = (fromFile < toFile) ? 1 : -1;

    for (char file = fromFile + step; file != toFile; file += step) {
        auto it = squares_.find(Position(rank, file));
        if (it != squares_.end() && it->second->isOccupied()) {
            return true; // If a square is occupied, then there's an obstruction
        }
    }
    return false;
};


std::unordered_set<Position> Board::getAttackedPositions_(Color color) const {
    std::unordered_set<Position> attackedPositions;

    for (const auto& [position, pSquare] : squares_) {
        const IPiece* piece = pSquare->getPiece();
        if (piece && piece->getColor() == color) {
            std::unordered_set<Position> possiblePositions = piece->getPossiblePositions(position);
            for (const Position& pos : possiblePositions) {
                attackedPositions.insert(pos);
            }
        }
    }
    return attackedPositions;
};


Position Board::findKing_(Color color) const {
    for (const auto& [position, pSquare] : squares_) {
        const IPiece* piece = pSquare->getPiece();
        if (piece && piece->getColor() == color && piece->getSymbol() == 'K') {
            return position;
        }
    }

    throw std::logic_error("No King located.");
    return Position(); //invalid position returned
};