#include <gmock/gmock.h>
#include "board_rules.h"

class MockBoardRules : public BoardRules {
public:
    MOCK_METHOD(bool, isValidMove, (const Board& board, const Move& move, const Move& previousMove), (override));
    MOCK_METHOD(bool, isInCheck, (const Board& board, const Color kingColor), (const, override));
    MOCK_METHOD(bool, isValidCastling, (const Board& board, const Move& kingMove), (const, override));
    MOCK_METHOD(bool, isValidEnPassant, (const Move& previousMove, const Move& move), (const, override));
    MOCK_METHOD(bool, isValidPromotion, (const Move& move), (const, override));

    MOCK_METHOD(std::unordered_set<Position>, generateValidPositions, (const Board& board, const IPiece* piece, const Position& from, const Move& previousMove), (override));

};