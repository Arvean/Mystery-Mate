#include "pawn.h"

std::unordered_set<Position> Pawn::getPossiblePositions(const Position& from) const {
    std::unordered_set<Position> possiblePositions;

    int forwardDirection = (color_ == Color::WHITE) ? 1 : -1;
    Position forwardPosition(from.getFile(), from.getRank() + forwardDirection);
    Position doubleForwardPosition(from.getFile(), from.getRank() + 2 * forwardDirection);

    possiblePositions.emplace(forwardPosition);

    if ((color_ == Color::WHITE && from.getRank() == 2) || (color_ == Color::BLACK && from.getRank() == GRID_SIZE - 1)) {
        possiblePositions.emplace(doubleForwardPosition);
    }

    for (int fileOffset : {-1, 1}) {
        Position capturePosition(from.getFile() + fileOffset, from.getRank() + forwardDirection);
        possiblePositions.emplace(capturePosition);
    }
    return possiblePositions;
};

bool Pawn::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};