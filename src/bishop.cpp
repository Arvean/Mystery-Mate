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
            int newFileInt = charToFile_(from.getFile()) + i * direction.first;
            int newRank = from.getRank() + i * direction.second;
            if (newFileInt >= 0 && newFileInt < GRID_SIZE && newRank >= 1 && newRank <= GRID_SIZE) {
                positions.emplace(fileToChar_(newFileInt), newRank);
            }
        }
    }

    return positions;
}

bool Bishop::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};