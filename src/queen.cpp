#include "queen.h"

std::unordered_set<Position> Queen::getPossiblePositions(const Position& from) const {
    std::unordered_set<Position> positions;

    // Directions for rank and file movements
    std::vector<std::pair<int, int>> rankFileDirections = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}
    };

    // Directions for diagonal movements
    std::vector<std::pair<int, int>> diagonalDirections = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    // Rank and file movements
    for (const auto& direction : rankFileDirections) {
        for (int i = 1; i < GRID_SIZE; ++i) {
            int newFile = charToFile_(from.getFile()) + i * direction.first;
            int newRank = from.getRank() + i * direction.second;
            if (newFile >= 0 && newFile < GRID_SIZE && newRank >= 0 && newRank < GRID_SIZE) {
                positions.emplace(fileToChar_(newFile), newRank);
            }
        }
    }

    // Diagonal movements
    for (const auto& direction : diagonalDirections) {
        for (int i = 1; i < GRID_SIZE; ++i) {
            int newFile = charToFile_(from.getFile()) + i * direction.first;
            int newRank = from.getRank() + i * direction.second;
            if (newFile >= 0 && newFile < GRID_SIZE && newRank >= 0 && newRank < GRID_SIZE) {
                positions.emplace(fileToChar_(newFile), newRank);
            }
        }
    }

    return positions;
}

bool Queen::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};