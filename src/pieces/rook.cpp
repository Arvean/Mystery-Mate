#include "rook.h"

Rook::Rook(Color color) 
    : color_(color), symbol_('R')
{};

std::unordered_set<Position> Rook::getPossibleMoves(const Position& from) const {
    std::unordered_set<Position> positions;

    // Directions for rank and file movements
    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}
    };

    // Rank and file movements
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
bool Rook::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};