#include "bishop.h"

Bishop::Bishop(Color color) 
    : color_(color), symbol_('B')
{};

std::unordered_set<Position> Bishop::getPossibleMoves(const Position& from) const {
    std::unordered_set<Position> positions;

    // Directions for diagonal movements
    std::vector<std::pair<int, int>> directions = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    // Diagonal movements
    for (const auto& direction : directions) {
        for (int i = 1; i < GRID_SIZE; ++i) {
            int newFile = from.getFile() + i * direction.first;
            int newRank = from.getRank() + i * direction.second;
            if (newFile >= 0 && newFile < GRID_SIZE && newRank >= 0 && newRank < GRID_SIZE) {
                positions.emplace(newFile, newRank);
            }
        }
    }

    return positions;
}

bool Bishop::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};