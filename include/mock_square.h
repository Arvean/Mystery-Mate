#include <gmock/gmock.h>
#include "square.h" 

class MockSquare : public Square {
public:
    MOCK_METHOD(bool, isOccupied, (), (const, override));
    MOCK_METHOD(const Position&, getPosition, (), (const, override));
    MOCK_METHOD(void, placePiece, (const IPiece* pPiece), (override));
    MOCK_METHOD(const IPiece*, getPiece, (), (const, override));
    MOCK_METHOD(void, removePiece, (), (override));
};