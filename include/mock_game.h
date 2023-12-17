#include <gmock/gmock.h>
#include "game.h"  /
class MockGame : public Game {
public:
    MockGame() : Game() {}
    MockGame(Player* player_1, Player* player_2, Board* board, BoardRules* boardRules)
        : Game(player_1, player_2, board, boardRules) {}

    // Mocked methods
    //MOCK_METHOD(void, startGame, (), (override));
    //MOCK_METHOD(void, movePiece, (const Move& move, const Player* pPlayer), (override));

    //MOCK_METHOD(GameState, getGameState, (), (const, override));
    //MOCK_METHOD(Player*, getCurrentPlayer, (), (const, override));
    //MOCK_METHOD(Board*, getBoard, (), (const, override));

    // OBE: MOCK_METHOD(bool, _isCheckmate, (Color color));
    MOCK_METHOD(bool, _isHorcruxCaptured, (int horcruxID));
    MOCK_METHOD(bool, _isStalemate, (), (const));
    MOCK_METHOD(bool, _hasInsufficientMaterial, (), (const));
    //MOCK_METHOD(void, _endGame, ());
};
