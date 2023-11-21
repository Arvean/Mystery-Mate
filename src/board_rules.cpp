#include "board_rules.h"
#include "rook.h"
#include "king.h"
#include <iostream> 


bool BoardRules::isValidMove(const Board& board, const Move& move, const Move& previousMove) {

    if (move.getPiece()->getType() == PieceType::KING) {
        if (isValidCastling(board, move)) { return true; }
    }
    if (move.getPiece()->getType() == PieceType::PAWN) {
        if (isValidPromotion(move)) { return true; }
    }

    Position to = move.getTo();
    std::unordered_set<Position> possiblePositions = move.getPiece()->getPossiblePositions(move.getFrom());

    // En Passant checked in addPawnCapturePositions in _availablePositions
    _availablePositions(board, possiblePositions, move.getFrom(), previousMove);

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
    // Check if the current piece is a pawn and is moving diagonally by one square
    if (!previousMove.getPiece()) {return false;}

    if (move.getPiece()->getType() != PieceType::PAWN ||
        abs(move.getTo().getFile() - move.getFrom().getFile()) != 1 || 
        abs(move.getTo().getRank() - move.getFrom().getRank()) != 1) {
        return false;
    }

    // Check if the previous move was a pawn moving two squares forward
    if (previousMove.getPiece()->getType() != PieceType::PAWN ||
        abs(previousMove.getTo().getRank() - previousMove.getFrom().getRank()) != 2 ||
        previousMove.getTo().getFile() != previousMove.getFrom().getFile()) {
        return false;
    }

    // Ensure the previous move's pawn is adjacent to the current pawn's starting position
    if (previousMove.getTo().getFile() != move.getFrom().getFile() ||
        abs(previousMove.getTo().getRank() - move.getFrom().getRank()) != 1) {
        return false;
    }

    return true;
}


bool BoardRules::isInCheck(const Board& board, const Color kingColor) const {
    const Position* kingPosition = board.findKing(kingColor);
    if (!kingPosition) {throw std::logic_error("Could not find King");}

    Color opponentColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    std::unordered_set<Position> attackedPositions = board.getAttackedPositions_(opponentColor);

    return attackedPositions.find(*kingPosition) != attackedPositions.end();
};


std::unordered_set<Position> BoardRules::generateValidPositions(const Board& board, const IPiece* piece, const Position& from, const Move& previousMove) {
    auto it = board.squares.find(from);
    if (it == board.squares.end()) {
        throw std::logic_error("Invalid starting position");
    }
    if (!it->second->isOccupied()) {
        throw std::logic_error("Current square is not occupied");
    }
    std::unordered_set<Position> pos = piece->getPossiblePositions(from);
    _availablePositions(board, pos, from, previousMove);
    return pos;
}


void BoardRules::_availablePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from, const Move& previousMove) {
    // Have to add castling, en passant to possible available moves set

    auto it = board.squares.find(from);
    if (it == board.squares.end()) {
        throw std::logic_error("Invalid starting position");
    }
    if (!it->second->isOccupied()) {
        throw std::logic_error("Current square is not occupied");
    }

     const auto pieceType = it->second->getPiece()->getType();

    if (pieceType == PieceType::PAWN) {
        _addPawnCapturePositions(board, possiblePositions, from, previousMove);
    } // Repeating in range and is occupied checks?}

    std::vector<Position> toRemove;
    std::unordered_set<Position> captures; // Note to send this to the frontend in a future update

    for (const auto& pos : possiblePositions) {
        std::cout << "Possible Positions: " << pos.getFile() << pos.getRank() << std::endl;
        if (pieceType != PieceType::KNIGHT && board.isObstructed_(pos)) {
            // Find direction from `from` to `pos`
            int deltaX = pos.getFile() - from.getFile();
            int deltaY = pos.getRank() - from.getRank();

            int stepSizeX = (deltaX != 0) ? (deltaX / abs(deltaX)) : 0;
            int stepSizeY = (deltaY != 0) ? (deltaY / abs(deltaY)) : 0;

            int currFile = pos.getFile();
            int currRank = pos.getRank();

            // Check the first obstructed position for a capture possibility
            auto obstructingSquareIt = board.squares.find(Position(currFile, currRank));
            if (obstructingSquareIt != board.squares.end() &&
                obstructingSquareIt->second->isOccupied() &&
                obstructingSquareIt->second->getPiece()->getColor() != it->second->getPiece()->getColor()) {
                captures.insert(pos); // This is a possible capture.
            } else {
                toRemove.push_back(pos); // Not a capture, so mark for removal.
            }

            // Move to the next position in the direction of the obstruction
            currFile += stepSizeX;
            currRank += stepSizeY;

            // Collect all positions beyond the first obstruction as they are not reachable
            while (board.isInsideBoard_(Position(currFile, currRank))) {
                toRemove.push_back(Position(currFile, currRank));
                currFile += stepSizeX;
                currRank += stepSizeY;
            }
        } else if (pieceType == PieceType::KNIGHT) {
            // Check if the knight's destination is capturable or empty
            auto destIt = board.squares.find(pos);
            if (destIt != board.squares.end() && destIt->second->isOccupied()) {
                if (destIt->second->getPiece()->getColor() != it->second->getPiece()->getColor()) {
                    captures.insert(pos); // This is a possible capture.
                } else {
                    toRemove.push_back(pos); // Square is occupied by a friendly piece, remove from moves.
                }
            }
            // If the destination square is not found in the map, it means it's unoccupied and already a valid move, so do nothing.
        }
    }

    // Now erase all collected positions.
    for (const auto& pos : toRemove) {
        possiblePositions.erase(pos);
    }
    
    if (it->second->getPiece()->getType() == PieceType::KING) {
        _removeKingInCheckPositions(board, possiblePositions, from);
        _removeKingInCheckPositions(board, captures, from);
    } // Repeating in range and is occupied checks?}

}


void BoardRules::_addPawnCapturePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from, const Move& lastMove) const {
    auto it = board.squares.find(from);
    if (it == board.squares.end()) { throw std::logic_error("Invalid starting position"); }
    if (!it->second->isOccupied()) { throw std::logic_error("Current square is not occupied"); }
    if (it->second->getPiece()->getType() != PieceType::PAWN) { throw std::logic_error("_addPawnCapturePositions is only valid for Pawns"); }

    Color pawnColor = it->second->getPiece()->getColor();
    int forwardDirection = (pawnColor == Color::WHITE) ? 1 : -1;

    // Standard diagonal captures
    for (int fileOffset : {-1, 1}) {
        char newFile = from.getFile() + fileOffset;
        int newRank = from.getRank() + forwardDirection;

        auto captureIt = board.squares.find(Position(newFile, newRank));
        if (captureIt != board.squares.end() && captureIt->second->isOccupied() &&
            captureIt->second->getPiece()->getColor() != pawnColor) {
            std::cout << "Add Pawn Capture Positions" << std::endl;
            possiblePositions.emplace(captureIt->second->getPosition());
        }
    }

    // En Passant capture
    Position epCaptureRight(from.getFile() + 1, from.getRank() + forwardDirection);
    Position epCaptureLeft(from.getFile() - 1, from.getRank() + forwardDirection);
    
    if (isValidEnPassant(lastMove, Move(it->second->getPiece(), from, epCaptureRight))) {
        int rankOffset = (pawnColor == Color::WHITE) ? 1 : -1;
        std::cout << "Valid EnPassant" << std::endl;
        possiblePositions.emplace(Position(epCaptureRight.getFile(), from.getRank() + rankOffset));
    }

    if (isValidEnPassant(lastMove, Move(it->second->getPiece(), from, epCaptureLeft))) {
        int rankOffset = (pawnColor == Color::WHITE) ? 1 : -1;
        std::cout << "Valid EnPassant" << std::endl;
        possiblePositions.emplace(Position(epCaptureLeft.getFile(), from.getRank() + rankOffset));
    }

    std::cout << "epCaptureRight File: " << epCaptureRight.getFile() << std::endl;
    std::cout << "epCaptureLeft File: " << epCaptureLeft.getFile() << std::endl;

    std::cout << "epCaptureRight Rank: " << epCaptureRight.getRank() << std::endl;
    std::cout << "epCaptureLeft Rank: " << epCaptureLeft.getRank() << std::endl;
}


void BoardRules::_removeKingInCheckPositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from) {
    auto it = board.squares.find(from);
    if (it == board.squares.end()) {throw std::logic_error("Invalid starting position");}
    if (!it->second->isOccupied()) {throw std::logic_error("Current square is not occupied");}
    if (it->second->getPiece()->getType() != PieceType::KING) {throw std::logic_error("_addPawnCapturePositions is only valid for King");}
    IPiece* pKing = const_cast<IPiece*> (it->second->getPiece());

    if (it->second->getPiece()->getType() == PieceType::KING) {
        auto it = possiblePositions.begin();
        while (it != possiblePositions.end()) {
            Position pos = *it;
            Board tempBoard = board;
            tempBoard.placePiece(pos, pKing);

            if (isInCheck(tempBoard, pKing->getColor())) {
                it = possiblePositions.erase(it);
            } else {++it;}
            tempBoard.removePiece(tempBoard.squares.at(pos).get());
        }
    } else {throw std::logic_error("removeKingInCheckMoves is only valid for King moves");}
}