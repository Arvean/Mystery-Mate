#include "board_rules.h"
#include "rook.h"
#include "king.h"
#include <iostream> 

bool BoardRules::isValidMove(const Board& board, const Move& move) const {
    if (!move.getPiece()->isValidMove(move)) {
        return false;
    }

    Position to = move.getTo();
    std::unordered_set<Position> possiblePositions = move.getPiece()->getPossiblePositions(move.getFrom());

    // Debugging prints can remain the same...

    for (const auto& pos : possiblePositions) {
        if (board.isObstructed_(pos)) {
            // Find direction from `move.getFrom()` to `pos`
            int deltaX = pos.getFile() - move.getFrom().getFile();
            int deltaY = pos.getRank() - move.getFrom().getRank();

            int stepSizeX = (deltaX != 0) ? (deltaX / abs(deltaX)) : 0; // if deltaX is 0, stepSizeX will also be 0
            int stepSizeY = (deltaY != 0) ? (deltaY / abs(deltaY)) : 0; // if deltaY is 0, stepSizeY will also be 0

            int currFile = pos.getFile() + stepSizeX;
            int currRank = pos.getRank() + stepSizeY;

            // Remove positions in the direction of the obstruction
            while (board.isInsideBoard_(Position(currFile, currRank))) {
                possiblePositions.erase(Position(currFile, currRank));
                currFile += stepSizeX;
                currRank += stepSizeY;
            }
        }
    }

    return possiblePositions.find(to) != possiblePositions.end();
};


bool BoardRules::isValidCastling(const Board& board, const Move& kingMove, const Move& rookMove) const {
    const King* king = dynamic_cast<const King*>(kingMove.getPiece());
    const Rook* rook = dynamic_cast<const Rook*>(rookMove.getPiece());

    if (king == nullptr) {std::logic_error("No king selected for castling");}
    if (rook == nullptr) {std::logic_error("No rook selected for castling");}

    if (king->getSymbol() != 'K' || rook->getSymbol() != 'R') {
        throw std::logic_error("Invalid parameter. King and rook expected.");
    }

    if (king->getHasMoved() || rook->getHasMoved()) {return false;}

    if (board.isObstructedBetweenFile_(kingMove.getFrom(), rookMove.getFrom())) {return false;}

    if (isInCheck(board, kingMove.getPiece())) {return false;}

    return true;
};


bool BoardRules::isValidPromotion(const Move& move, IPiece* newPiece) const {
    if (move.getPiece()->getSymbol() != 'P') return false;

    int targetRank = move.getPiece()->getColor() == Color::WHITE ? GRID_SIZE : 0;
    int promotionRank = move.getPiece()->getColor() == Color::BLACK ? 1 : 0;

    if (move.getTo().getRank() != targetRank && move.getTo().getRank() != promotionRank) {return false;}

    char newSymbol = newPiece->getSymbol();

    return (newSymbol == 'Q' || newSymbol == 'R' || newSymbol == 'B' || newSymbol == 'N');
};


void BoardRules::addPawnCapturePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from) {

    auto it = board.squares_.find(from);
    if (it != board.squares_.end()) {std::logic_error("Invalid starting position");}
    if (!it->second->isOccupied()) {std::logic_error("Current square is not occupied");}

    Color pawnColor = it->second->getPiece()->getColor();

    int forwardDirection = (pawnColor == Color::WHITE) ? 1 : -1;

    for (int fileOffset : {-1, 1}) {
        char newFile = from.getFile() + fileOffset;
        int newRank = from.getRank() + forwardDirection;

        // Add the capture position if an opponent's piece is present on that square
        auto it = board.squares_.find(Position(newFile, newRank));
        if (it != board.squares_.end()) {std::logic_error("Invalid starting position");}
        if (!it->second->isOccupied()) {std::logic_error("Current square is not occupied");}

        if (it->second->getPiece()->getColor() != pawnColor) {
            possiblePositions.emplace(it->second->getPosition());
        }
    }
}


bool BoardRules::isValidEnPassant(const Move& previousMove, const Move& move) const {
    if (move.getPiece()->getSymbol() != 'P') return false;

    if (abs(move.getTo().getFile() - move.getFrom().getFile()) != 1 || 
        abs(move.getTo().getRank() - move.getFrom().getRank()) != 1) return false;

    // Check if pawn double move in a file += 1 was made previously
    if (previousMove.getPiece()->getSymbol() != 'P') return false;

    if (abs(previousMove.getFrom().getRank() - move.getFrom().getRank()) != 2 &&
        abs(previousMove.getFrom().getFile() - move.getFrom().getFile()) != 1) return false;

    if (abs(previousMove.getTo().getRank() - move.getFrom().getRank()) != 0 &&
        abs(previousMove.getTo().getFile() - move.getFrom().getFile()) != 1) return false;

    return true;
};


bool BoardRules::isInCheck(const Board& board, const IPiece* king) const {
    Position kingPosition = board.findKing_(king->getColor());

    Color opponentColor = (king->getColor() == Color::WHITE) ? Color::BLACK : Color::WHITE;
    std::unordered_set<Position> attackedPositions = board.getAttackedPositions_(opponentColor);

    return attackedPositions.find(kingPosition) != attackedPositions.end();
};
