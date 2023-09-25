#include "gtest/gtest.h"
#include "game.h"
#include "mock_board.h"
#include "mock_board_rules.h"
#include "mock_player.h"
#include "mock_piece.h"
#include "mock_square.h"
#include "mock_game.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

class GameTest : public ::testing::Test {
protected:
    std::unique_ptr<MockBoard> mockBoard;
    std::unique_ptr<MockBoardRules> mockRules;
    std::unique_ptr<MockPlayer> mockPlayer1;
    std::unique_ptr<MockPlayer> mockPlayer2;
    std::unique_ptr<MockSquare> mockSquare;
    std::unique_ptr<MockPiece> mockPiece;
    std::unique_ptr<Game> game; // Notice the type change to MockGame


    void SetUp() override {
        mockPiece = std::make_unique<MockPiece>();
        mockBoard = std::make_unique<MockBoard>();
        mockRules = std::make_unique<MockBoardRules>();
        mockSquare = std::make_unique<MockSquare>();
        mockPlayer1 = std::make_unique<MockPlayer>(Color::WHITE);
        mockPlayer2 = std::make_unique<MockPlayer>(Color::BLACK);


        ON_CALL(*mockBoard, getSquare(_))
        .WillByDefault(Return(mockSquare.get()));

        ON_CALL(*mockSquare, getPiece())
        .WillByDefault(Return(mockPiece.get()));

        ON_CALL(*mockPlayer1, getColor())
        .WillByDefault(Return(Color::WHITE));

        ON_CALL(*mockPlayer2, getColor())
        .WillByDefault(Return(Color::BLACK));

        ON_CALL(*mockPlayer1, getHorcruxeID())
        .WillByDefault(Return(INVALID_HORCRUXE_ID));

        ON_CALL(*mockPlayer2, getHorcruxeID())
        .WillByDefault(Return(INVALID_HORCRUXE_ID));

        game = std::make_unique<Game>(mockPlayer1.get(), mockPlayer2.get(), mockBoard.get(), mockRules.get());
    }
};


TEST_F(GameTest, StartGameInitializesProperly) {
    game->startGame();
    EXPECT_EQ(game->getGameState(), GameState::WHITE_MOVE);
    EXPECT_EQ(game->getCurrentPlayer()->getColor(), Color::WHITE);
}


TEST_F(GameTest, MovePieceMakesAValidMove) {
    MockPiece mockPiece;
    Position from('d', 4);
    
    Move someMove(&mockPiece, from, Position('f', 6));

    EXPECT_CALL(*mockRules, isValidMove(_, _, _)).WillOnce(Return(true));
    EXPECT_CALL(*mockSquare, placePiece(_)).Times(AtLeast(1));

    game->movePiece(someMove, mockPlayer1.get());
}


TEST_F(GameTest, GameOverCheck) {
    bool isOver = game->checkGameOver();

    EXPECT_TRUE(isOver);
}
