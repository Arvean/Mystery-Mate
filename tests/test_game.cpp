#include "gtest/gtest.h"
#include "game.h"
#include "mock_board.h"
#include "mock_board_rules.h"
#include "mock_player.h"
#include "mock_piece.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

class GameTest : public ::testing::Test {
protected:
    std::unique_ptr<MockBoard> mockBoard;
    std::unique_ptr<MockBoardRules> mockRules;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    std::unique_ptr<Game> game;

    void SetUp() override {
        mockBoard = std::make_unique<MockBoard>();
        mockRules = std::make_unique<MockBoardRules>();
        player1 = std::make_unique<MockPlayer>();
        player2 = std::make_unique<MockPlayer>();
        game = std::make_unique<Game>(*player1, *player2);
    }
};


TEST_F(GameTest, StartGameInitializesProperly) {
    EXPECT_EQ(game->getGameState(), GameState::IN_PROGRESS);
    EXPECT_EQ(game->getCurrentPlayer().getColor(), Color::WHITE);
    game->startGame();
}


TEST_F(GameTest, EndGameChangesState) {
    game->endGame();
    EXPECT_EQ(game->getGameState(), GameState::ENDED);
}


TEST_F(GameTest, MovePieceMakesAValidMove) {
    MockPiece mockPiece;
    Position from('d', 4);
    
    Move someMove(&mockPiece, from, Position('f', 6));

    EXPECT_CALL(*mockRules, isValidMove(_, _, _)).WillOnce(Return(true));
    EXPECT_CALL(*mockBoard, placePiece(_, _)).Times(AtLeast(1));
    game->movePiece(someMove);
}


TEST_F(GameTest, GameOverCheck) {
    EXPECT_CALL(*mockRules, isInCheck(_, _)).WillOnce(Return(false));
    bool isOver = game->isGameOver();
    EXPECT_FALSE(isOver);  // Assuming game is not over if not in check, for simplicity
}


TEST_F(GameTest, SwitchPlayerChangesPlayer) {
    Player initialPlayer = game->getCurrentPlayer();
    game->switchPlayer();
    EXPECT_NE(game->getCurrentPlayer().getColor(), initialPlayer.getColor());
}