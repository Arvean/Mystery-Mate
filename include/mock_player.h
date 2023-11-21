#include <gmock/gmock.h>
#include "player.h" 

class MockPlayer : public Player {
public:
    MockPlayer() : Player() {}
    MockPlayer(const Color color) : Player(color) {}

    MOCK_METHOD(int, getHorcruxID, (), (const, override));
    MOCK_METHOD(Color, getColor, (), (const, override));
};