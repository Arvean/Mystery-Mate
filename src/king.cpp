#include "king.h"

std::unordered_set<Position> King::getPossiblePositions(const Position& from) const {
    std::unordered_set<Position> possiblePositions;

    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, // Vertical and horizontal
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonal
    };

    // Check each direction, and add the position if it's within the bounds of the board
    for (const auto& dir : directions) {
        int newFile = charToFile(from.getFile()) + dir.first;
        int newRank = from.getRank() + dir.second;
        if (newFile >= charToFile('a') && newFile <= charToFile('h') && newRank >= 1 && newRank <= GRID_SIZE) {
            possiblePositions.emplace(fileToChar(newFile), newRank);
        }
    }

    return possiblePositions;
};

bool King::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};