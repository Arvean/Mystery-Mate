#include "pawn.h"

Pawn::Pawn(Color color) 
    : hasMoved_(false), color_(color), symbol_('P')
{};

std::unordered_set<Position> Pawn::getPossiblePositions(const Position& from) const {
    std::unordered_set<Position> possiblePositions;

    int forwardDirection = (color == Color::WHITE) ? 1 : -1;
    Position forwardPosition(from.getFile(), from.getRank() + forwardDirection);
    Position doubleForwardPosition(from.getFile(), from.getRank() + 2 * forwardDirection);

    possiblePositions.emplace(forwardPosition);

    if ((color == Color::WHITE && from.getRank() == 2) || (color == Color::BLACK && from.getRank() == GRID_SIZE - 1)) {
        possiblePositions.emplace(doubleForwardMove);
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