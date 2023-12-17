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

    // Validate that the piece is a king
    if (king == nullptr || king->getType() != PieceType::KING) {
        throw std::logic_error("Invalid piece. King expected for castling.");
    }

    // Check if the king or rook has moved, or if the king is in check
    if (king->getHasMoved() || isInCheck(board, king->getColor())) {
        return false;
    }

    Position kingTo = kingMove.getTo();
    Position rookFrom, rookTo;
    bool isKingSide = false;

    // Determine castling type and set rook positions
    if (king->getColor() == Color::WHITE) {
        isKingSide = (kingTo == Position('g', 1));
        rookFrom = isKingSide ? Position('h', 1) : Position('a', 1);
        rookTo = isKingSide ? Position('f', 1) : Position('d', 1);
    } else { // Black
        isKingSide = (kingTo == Position('g', 8));
        rookFrom = isKingSide ? Position('h', 8) : Position('a', 8);
        rookTo = isKingSide ? Position('f', 8) : Position('d', 8);
    }

    Square* rookSquare = board.getSquare(rookFrom);
    if (rookSquare == nullptr || rookSquare->getPiece() == nullptr ||
        rookSquare->getPiece()->getType() != PieceType::ROOK ||
        dynamic_cast<const Rook*>(rookSquare->getPiece())->getHasMoved()) {
        return false;
    }

    // Ensure no pieces are obstructing the path between the king and rook
    if (board.isObstructedBetweenFile_(kingMove.getFrom(), rookSquare->getPosition())) {
        return false;
    }

    // Check if the squares the king passes through are under attack
    for (int file = std::min(kingMove.getFrom().getFile(), kingTo.getFile());
         file <= std::max(kingMove.getFrom().getFile(), kingTo.getFile()); ++file) {
        Position positionToCheck(static_cast<char>(file), kingMove.getFrom().getRank());
        if (board.isAttackedPosition(positionToCheck, king->getColor())) {
            return false;
        }
    }

    return true;
}


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

void BoardRules::_addKingCastlingPositions(const Board& board, std::unordered_set<Position>& possiblePositions, const King* king, const Position& from) const {
    // White King Position
    if (from == Position('e', 1)) {
        Position whiteLongCastlePosition = Position('c', 1);
        Position whiteShortCastlePosition = Position('g', 1);
        if (isValidCastling(board, Move(king, from, whiteLongCastlePosition))) {
            possiblePositions.insert(whiteLongCastlePosition);
        }
        if (isValidCastling(board, Move(king, from, whiteShortCastlePosition))) {
            possiblePositions.insert(whiteShortCastlePosition);
        }
    // Black King Position
    } else if (from == Position('e', 8)) {
        Position blackLongCastlePosition = Position('c', 8);
        Position blackShortCastlePosition = Position('g', 8);
        if (isValidCastling(board, Move(king, from, blackLongCastlePosition))) {
            possiblePositions.insert(blackLongCastlePosition);
        }
        if (isValidCastling(board, Move(king, from, blackShortCastlePosition))) {
            possiblePositions.insert(blackShortCastlePosition);
        }
    }
}

void BoardRules::_availablePositions(const Board& board, std::unordered_set<Position>& possiblePositions, const Position& from, const Move& previousMove) {
    // Have to add castling, en passant to possible available moves set
    // TODO: Move to Board class

    auto it = board.squares.find(from);
    if (it == board.squares.end()) {
        throw std::logic_error("Invalid starting position");
    }
    if (!it->second->isOccupied()) {
        throw std::logic_error("Current square is not occupied");
    }
    
    const IPiece* piece = it->second->getPiece();
    const auto pieceType = piece->getType();

    if (pieceType == PieceType::PAWN) {
        _addPawnCapturePositions(board, possiblePositions, from, previousMove);
    } // Repeating in range and is occupied checks?}

    if (pieceType == PieceType::KING) {
        _addKingCastlingPositions(board, possiblePositions, dynamic_cast<const King*>(piece), from);
    }

    std::vector<Position> toRemove;
    std::unordered_set<Position> captures; // Note to send this to the frontend in a future update

    for (const auto& pos : possiblePositions) {
        std::cout << "Possible Positions: " << pos.getFile() << pos.getRank() << std::endl;
        if (pieceType != PieceType::KNIGHT && board.getSquare(pos)->getPiece()) {
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