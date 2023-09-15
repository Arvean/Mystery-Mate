#include <gmock/gmock.h>
#include "board.h"  

class MockBoard : public Board {
public:

    MOCK_METHOD(void, placePiece, (const Position& position, IPiece* piece), ());

    MOCK_METHOD(Square*, getSquare, (const Position& position), (const, override));
    MOCK_METHOD(void, removePiece, (Square* pSquare), (override));
};

