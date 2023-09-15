#include "board.h"

Board::Board() {
    for(int rank = 1; rank <= GRID_SIZE; ++rank) {
        for(char file = 'a'; file <= 'h'; ++file) {
            squares[Position(file, rank)] = std::make_unique<Square>(Position(rank, file));
        }
    }
};


Board::Board(const Board& other) {
    for (int rank = 1; rank <= GRID_SIZE; ++rank) {
        for (char file = 'a'; file <= 'h'; ++file) {
            Position pos(file, rank);
            squares[pos] = std::make_unique<Square>(pos);
            if (other.squares.at(pos)->getPiece()) {
                IPiece* pClonedPiece = other.squares.at(pos)->getPiece()->clone();
                squares.at(pos)->placePiece(pClonedPiece);
            }
        }
    }
};


Board& Board::operator=(const Board& other) {
    // Handle self-assignment
    if (this == &other) {return *this;}

    // Cleanup any existing data/resources if necessary.
    // Given the existing code, this primarily means clearing your current `squares` map.
    squares.clear();

    // Copy the data/resources from the other board.
    for (int rank = 1; rank <= GRID_SIZE; ++rank) {
        for (char file = 'a'; file <= 'h'; ++file) {
            Position pos(file, rank);

            // Create a new unique Square for this position
            squares[pos] = std::make_unique<Square>(pos);

            // If the other board's square at this position has a piece
            if (other.squares.at(pos)->getPiece()) {
                // Clone the piece and place it on the current board
                IPiece* pClonedPiece = other.squares.at(pos)->getPiece()->clone();
                squares.at(pos)->placePiece(pClonedPiece);
            }
        }
    }

    return *this;
}


void Board::placePiece(const Position& position, IPiece* piece) {
    auto it = squares.find(position);
    if (it != squares.end()) {
        it->second->placePiece(piece);
    } else {
        throw std::logic_error("Invalid square position");
    }
};


Square* Board::getSquare(const Position& position) const {
    auto it = squares.find(position);
    if (it != squares.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
};

void Board::removePiece(Square* pSquare) {
    if (pSquare->isOccupied()) {
        pSquare->removePiece();
    } else {
        throw std::logic_error("Invalid. No piece to remove.");
    }
};


bool Board::isInsideBoard_(const Position& position) const {
    return squares.find(position) != squares.end();
}


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
        auto it = squares.find(Position(rank, file));
        if (it != squares.end() && it->second->isOccupied()) {
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
        auto it = squares.find(Position(rank, file));
        if (it != squares.end() && it->second->isOccupied()) {
            return true; // If a square is occupied, then there's an obstruction
        }
    }
    return false;
};


std::unordered_set<Position> Board::getAttackedPositions_(Color color) const {
    std::unordered_set<Position> attackedPositions;

    for (const auto& [position, pSquare] : squares) {
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
    for (const auto& [position, pSquare] : squares) {
        const IPiece* piece = pSquare->getPiece();
        if (piece && piece->getColor() == color && piece->getType() == PieceType::KING) {
            return position;
        }
    }

    throw std::logic_error("No King located.");
    return Position(); //invalid position returned
};


bool Board::isLightSquare(const Position& pos) {
    return (IPiece::charToFile(pos.getFile()) + pos.getRank()) % 2 == 1;
}

bool Board::isDarkSquare(const Position& pos) {
    return !isLightSquare(pos);
}