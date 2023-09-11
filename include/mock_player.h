#include <gmock/gmock.h>
#include "player.h" 

class MockPlayer : public Player {
public:
    MockPlayer() : Player() {}
    MockPlayer(int horcruxe_id, const Color color) : Player(horcruxe_id, color) {}

    MOCK_METHOD(int, getHorcruxeID, (), (const, override));
    MOCK_METHOD(Color, getColor, (), (const, override));
};