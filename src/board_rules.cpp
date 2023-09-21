#include "board_rules.h"
#include "rook.h"
#include "king.h"
#include <iostream> 

bool BoardRules::isValidMove(const Board& board, const Move& move, const Move* pPreviousMove) const {

    if (move.getPiece()->getType() == PieceType::KING) {
        if (isValidCastling(board, move)) { return true; }
    }
    if (move.getPiece()->getType() == PieceType::PAWN) {
        if (pPreviousMove) {
            if (isValidEnPassant(*pPreviousMove, move)) { return true; }
        }
        if (isValidPromotion(move)) { return true; }
    }
    if (!move.getPiece()->isValidMove(move)) {return false;}

    Position to = move.getTo();
    std::unordered_set<Position> possiblePositions = move.getPiece()->getPossiblePositions(move.getFrom());

    if (move.getPiece()->getType() == PieceType::PAWN) {_addPawnCapturePositions(board, possiblePositions, move.getFrom());} // Repeating in range and is occupied checks?}
    if (move.getPiece()->getType() == PieceType::KING) {_removeKingInCheckMoves(board, possiblePositions, move);} // Repeating in range and is occupied checks?}


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


bool BoardRules::isValidCastling(const Board& board, const Move& kingMove) const {
    const King* king = dynamic_cast<const King*>(kingMove.getPiece());
    
    if (king == nullptr) {
        throw std::logic_error("No king selected for castling");
    }

    if (king->getType() != PieceType::KING) {
        throw std::logic_error("Invalid parameter. King expected.");
    }

    if (king->getHasMoved()) {
        return false;
    }

    if (isInCheck(board, kingMove.getPiece()->getColor())) {
        return false;
    }

    Position kingTo = kingMove.getTo();
    Square* rookSquare = nullptr;

    // Check if it's a king's side castle
    if (king->getColor() == Color::WHITE && kingTo == Position('g', 1)) {
        rookSquare = board.getSquare(Position('h', 1));
    } else if (king->getColor() == Color::BLACK && kingTo == Position('g', 8)) {
        rookSquare = board.getSquare(Position('h', 8));
    }
    // Check if it's a queen's side castle
    else if (king->getColor() == Color::WHITE && kingTo == Position('c', 1)) {
        rookSquare = board.getSquare(Position('a', 1));
    } else if (king->getColor() == Color::BLACK && kingTo == Position('c', 8)) {
        rookSquare = board.getSquare(Position('a', 8));
    } else {return false;}

    // Verify the rook's status
    if (rookSquare == nullptr || rookSquare->getPiece() == nullptr || rookSquare->getPiece()->getType() != PieceType::ROOK) {
        return false;
    }

    const Rook* rook = dynamic_cast<const Rook*>(rookSquare->getPiece());
    if (rook == nullptr || rook->getHasMoved()) {return false;}

    // Ensure no pieces are obstructing the path between the king and rook
    if (board.isObstructedBetweenFile_(kingMove.getFrom(), rookSquare->getPosition())) {
        return false;
    }
    if (isInCheck(board, kingMove.getPiece()->getColor())) {return false;}

    return true;
};


bool BoardRules::isValidPromotion(const Move& move) const {
    if (move.getPiece()->getType() != PieceType::PAWN) return false;

    int targetRank = move.getPiece()->getColor() == Color::WHITE ? GRID_SIZE : 0;
    int promotionRank = move.getPiece()->getColor() == Color::BLACK ? 1 : 0;

    if (move.getTo().getRank() != targetRank && move.getTo().getRank() != promotionRank) {return false;}

    return true;
};


bool BoardRules::isValidEnPassant(const Move& previousMove, const Move& move) const {
    if (move.getPiece()->getType() != PieceType::PAWN) return false;

    if (abs(move.getTo().getFile() - move.getFrom().getFile()) != 1 || 
        abs(move.getTo().getRank() - move.getFrom().getRank()) != 1) return false;

    // Check if pawn double move in a file += 1 was made previously
    if (previousMove.getPiece()->getType() != PieceType::PAWN) return false;

    if (abs(previousMove.getFrom().getRank() - move.getFrom().getRank()) != 2 &&
        abs(previousMove.getFrom().getFile() - move.getFrom().getFile()) != 1) return false;

    if (abs(previousMove.getTo().getRank() - move.getFrom().getRank()) != 0 &&
        abs(previousMove.getTo().getFile() - move.getFrom().getFile()) != 1) return false;

    return true;
};


bool BoardRules::isInCheck(const Board& board, const Color kingColor) const {
    Position kingPosition = board.findKing_(kingColor);

    Color opponentColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    std::unordered_set<Position> attackedPositions = board.getAttackedPositions_(opponentColor);

    return attackedPositions.find(kingPosition) != attackedPositions.end();
};


void BoardRules::_addPawnCapturePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from) const {

    auto it = board.squares.find(from);
    if (it != board.squares.end()) {std::logic_error("Invalid starting position");}
    if (!it->second->isOccupied()) {std::logic_error("Current square is not occupied");}

    Color pawnColor = it->second->getPiece()->getColor();

    int forwardDirection = (pawnColor == Color::WHITE) ? 1 : -1;

    for (int fileOffset : {-1, 1}) {
        char newFile = from.getFile() + fileOffset;
        int newRank = from.getRank() + forwardDirection;

        // Add the capture position if an opponent's piece is present on that square
        auto it = board.squares.find(Position(newFile, newRank));
        if (it != board.squares.end()) {std::logic_error("Invalid starting position");}
        if (!it->second->isOccupied()) {std::logic_error("Current square is not occupied");}

        if (it->second->getPiece()->getColor() != pawnColor) {
            possiblePositions.emplace(it->second->getPosition());
        }
    }
}


void BoardRules::_removeKingInCheckMoves(const Board& board, std::unordered_set<Position>& possiblePositions, const Move& move) const {
    auto it = possiblePositions.begin();
    while (it != possiblePositions.end()) {
        Position pos = *it;
        Board tempBoard = board;
        tempBoard.placePiece(move.getTo(), const_cast<IPiece*>(move.getPiece()));

        if (isInCheck(tempBoard, tempBoard.getSquare(move.getFrom())->getPiece()->getColor())) {
            it = possiblePositions.erase(it);
        } else {++it;}
        tempBoard.removePiece(tempBoard.squares.at(move.getTo()).get());
    }
}