#include "board_rules.h"
#include "rook.h"
#include "king.h"


bool BoardRules::isValidMove(const Board& board, const Move& move) const {
    if (!move.getPiece()->isValidMove(move)) {
        return false;
    }

    Position to = move.getTo();
    std::unordered_set<Position> possiblePositions = move.getPiece()->getPossiblePositions(move.getFrom());

    for (auto it = possiblePositions.begin(); it != possiblePositions.end();) {
        if (board.isObstructed_(*it)) {
            it = possiblePositions.erase(it);
        } else {
            ++it;
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

    if (board.isObstructedBetweenRank_(kingMove.getFrom(), rookMove.getFrom())) {return false;}

    if (isInCheck(board, kingMove.getPiece())) {return false;}

    return true;
};


bool BoardRules::isValidPromotion(const Move& move, IPiece* newPiece) const {
    if (move.getPiece()->getSymbol() != 'P') return false;

    int targetRank = move.getPiece()->getColor() == Color::WHITE ? GRID_SIZE : 0;
    int promotionRank = move.getPiece()->getColor() == Color::BLACK ? 1 : 0;

    if (move.getTo().getRank() != targetRank || move.getTo().getRank() != promotionRank) {return false;}

    char newSymbol = newPiece->getSymbol();
    return (newSymbol == 'Q' || newSymbol == 'R' || newSymbol == 'B' || newSymbol == 'N');
};


bool BoardRules::isValidEnPassant(const Move& previousMove, const Move& move, const Position& capturedPawnPosition) const {
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
