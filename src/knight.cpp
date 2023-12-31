#include "knight.h"

std::unordered_set<Position> Knight::getPossiblePositions(const Position& from) const {
    std::unordered_set<Position> positions;

    // Define the possible moves for the knight
    std::vector<std::pair<int, int>> moves = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    // Check each move, and add the position if it's within the bounds of the board
    for (const auto& move : moves) {
        int newFile = charToFile(from.getFile()) + move.first;
        int newRank = from.getRank() + move.second;
        if (newFile >= charToFile('a') && newFile <= charToFile('h') && newRank >= 1 && newRank <= GRID_SIZE) {
            positions.emplace(fileToChar(newFile), newRank);
        }
    }

    return positions;
}

bool Knight::isValidMove(const Move& move) const {
    std::unordered_set possiblePositions = getPossiblePositions(move.getFrom());

    return possiblePositions.find(move.getTo()) != possiblePositions.end();
};