#include "board_rules.h"


bool BoardRules::isValidMove(const Move& move) const {
    if (!move.getPiece()->isValidMove(move)) {
        return false;
    }

    Position to = move.getTo();
    std::unordered_set<Position> possiblePositions = move.getPiece()->getPossiblePositions(move.getFrom());

    for (auto it = possiblePositions.begin(); it != possiblePositions.end();) {
        if (board_.isObstructed_(*it)) {
            it = possiblePositions.erase(it);
        } else {
            ++it;
        }
    }
    return possiblePositions.find(to) != possiblePositions.end();
}


bool BoardRules::isValidCastling(const Move& kingMove, const Move& rookMove) const {
    Piece* king = kingMove.getPiece();
    Piece* rook = rookMove.getPiece();

    if (king->getSymbol() != 'K' || rook->getSymbol() != 'R') {
        throw std::logic_error("Invalid parameter. King and rook expected.");
    }

    if (king->hasMoved() || rook->hasMoved()) return false;

    if (board_.isObstructedBetweenRank_(kingMove.getFrom().getRank(), rookMove.getFrom().getRank())) return false;

    if (isInCheck(kingMove.getPiece()->getColor())) return false;

    return true;
}


bool BoardRules::isValidPromotion(const Move& move, Piece* newPiece) const {
    if (move.getPiece()->getSymbol() != 'P') return false;

    int targetRank = move.getPiece()->getColor() == Color::WHITE ? GRID_SIZE : 0;
    int promotionRank = move.getPiece()->getColor() == Color::BLACK ? 1 : 0;

    if (move.getTo().getRank() != targetRank || move.getTo().getRank() != promotionRank) return false;

    char newSymbol = newPiece->getSymbol();
    return (newSymbol == 'Q' || newSymbol == 'R' || newSymbol == 'B' || newSymbol == 'N');
}


bool BoardRules::isValidEnPassant(const Move& move, const Position& capturedPawnPosition) const {
    if (move.getPiece()->getSymbol() != 'P') return false;

    if (abs(move.getTo().getFile() - move.getFrom().getFile()) != 1 || 
        abs(move.getTo().getRank() - move.getFrom().getRank()) != 1) return false;

    return capturedPawnPosition == board_.getPreviousPawnDoubleMove();
}


bool BoardRules::isInCheck(const Piece* king) const {
    Position& kingPosition = board_.findKing_(kingColor); // Assuming findKing method exists in Board

    Color opponentColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    std::unordered_set<Position> attackedPositions = getAttackedPositions_(opponentColor); // Assuming this method exists

    return attackedPositions.find(kingPosition) != attackedPositions.end();
}


bool BoardRules::isObstructed_(Position& position) const {
    if (getPiece(position) != nullptr) {
        return false;
    }
    return true;
};


bool BoardRules::isObstructedBetweenRank_(const Position& from, const Position& to) {
    int fromRank = from.getRank();
    int toRank = to.getRank();
    char file = from.getFile();

    int step = (fromRank < toRank) ? 1 : -1;

    for (int rank = fromRank + step; rank != toRank; rank += step) {
        auto it = squares_.find(Position(rank, file));
        if (it != squares_.end() && it->second.isOccupied()) {
            return true; // If a square is occupied, then there's an obstruction
        }
    }
    return false;
};


bool BoardRules::isObstructedBetweenFile_(const Position& from, const Position& to) {
    char fromFile = from.getFile();
    char toFile = to.getFile();
    int rank = from.getRank();

    int step = (fromFile < toFile) ? 1 : -1;

    for (char file = fromFile + step; file != toFile; file += step) {
        auto it = squares_.find(Position(rank, file));
        if (it != squares_.end() && it->second.isOccupied()) {
            return true; // If a square is occupied, then there's an obstruction
        }
    }
    return false;
};


std::unordered_set<Position> BoardRules::getAttackedPositions_(Color color) const {
    std::unordered_set<Position> attackedPositions;

    for (const auto& [position, square] : squares_) {
        Piece* piece = square.getPiece();
        if (piece && piece->getColor() == color) {
            std::unordered_set<Position> possiblePositions = piece->getPossiblePositions(position);
            for (const Position& pos : possiblePositions) {
                attackedPositions.insert(pos);
            }
        }
    }
    return attackedPositions;
};


Position BoardRules::findKing_(Color color) const {
    for (const auto& [position, square] : squares_) {
        Piece* piece = square.getPiece();
        if (piece && piece->getColor() == color && piece->getSymbol() = 'K') {
            return position;
        }
    }

    throw std::logic_error("No King located.");
    return Position(); //invalid position returned
}
