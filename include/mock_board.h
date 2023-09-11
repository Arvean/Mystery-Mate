#include <gmock/gmock.h>
#include "board.h"  

class MockBoard : public Board {
public:

    MOCK_METHOD(void, placePiece, (const Position& position, IPiece* piece), (override));

    MOCK_METHOD(Square*, getSquare, (const Position& position) const, (override));
    MOCK_METHOD(void, removePiece, (Square* pSquare), (override));

    virtual bool isLightSquareWrapper(const Position& pos) { return Board::isLightSquare(pos); }
    virtual bool isDarkSquareWrapper(const Position& pos) { return Board::isDarkSquare(pos); }
    
    MOCK_METHOD(bool, isLightSquareWrapper, (const Position& pos), ());
    MOCK_METHOD(bool, isDarkSquareWrapper, (const Position& pos), ());
};

