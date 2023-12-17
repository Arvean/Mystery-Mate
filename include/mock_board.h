#include <gmock/gmock.h>
#include "board.h"  

class MockBoard : public Board {
public:

    //MOCK_METHOD(void, placePiece, (const Position& position, const IPiece* piece), ());
    MOCK_METHOD(Square*, getSquare, (const Position& position), (const, override));
    MOCK_METHOD(void, removePiece, (Square* pSquare), (override));
    MOCK_METHOD(Position*, findKing, (Color color), (const, override));
    MOCK_METHOD(bool, isAttackedPosition, (const Position& position, const Color playerColor), (const, override));

};