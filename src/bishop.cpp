#include "bishop.h"

std::unordered_set<Position> Bishop::getPossiblePositions(const Position& from) const {
    std::unordered_set<Position> positions;

    // Directions for diagonal movements
    std::vector<std::pair<int, int>> directions = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    // Diagonal movements
    for (const auto& direction : directions) {
        for (int i = 1; i < GRID_SIZE; ++i) {
            int newFile = charToFile(from.getFile()) + i * direction.first;
            int newRank = from.getRank() + i * direction.second;
            if (newFile >= charToFile('a') && newFile <= charToFile('h') && newRank >= 1 && newRank <= GRID_SIZE) {
                positions.emplace(fileToChar(newFile), newRank);
            }
        }
    }

    return positions;
}

bool Bishop::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};