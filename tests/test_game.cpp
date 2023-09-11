#include <gtest/gtest.h>
#include "game.h"
#include "mock_board.h"
#include "mock_board_rules.h"
#include "mock_player.h"
#include "mock_piece.h"

class GameTest : public ::testing::Test {
protected:
    std::unique_ptr<MockBoard> mockBoard;
    std::unique_ptr<MockBoardRules> mockRules;
    std::unique_ptr<Game> game;

    void SetUp() override {
        mockBoard = std::make_unique<MockBoard>();
        mockRules = std::make_unique<MockBoardRules>();
        game = std::make_unique<Game>(std::move(mockBoard), std::move(mockRules));
    }
};


TEST_F(GameTest, StartGameInitializesProperly) {
    EXPECT_CALL(*mockBoard, _setupBoard()).Times(AtLeast(1));
    EXPECT_EQ(game->getGameState(), GameState::IN_PROGRESS);
    EXPECT_EQ(game->getCurrentPlayer().getColor(), Color::WHITE);
    game->startGame();
}

    currentPlayer_ = Color::WHITE; // Current player white
    gameState_ = GameState::IN_PROGRESS;


TEST_F(GameTest, EndGameChangesState) {
    game->endGame();
    EXPECT_EQ(game->getGameState(), GameState::ENDED);
}


TEST_F(GameTest, MovePieceMakesAValidMove) {
    MockPiece mockPiece;
    Position from('d', 4);
    
    Move someMove(&mockPiece, from, Position('f', 6));

    EXPECT_CALL(*mockRules, isValidMove(_, _)).WillOnce(Return(true));
    EXPECT_CALL(*mockBoard, placePiece(_, _)).Times(AtLeast(1));
    game->movePiece(someMove);
}


TEST_F(GameTest, GameOverCheck) {
    EXPECT_CALL(*mockRules, isInCheck(_, _)).WillOnce(Return(false));
    bool isOver = game->isGameOver();
    EXPECT_FALSE(isOver);  // Assuming game is not over if not in check, for simplicity
}


TEST_F(GameTest, SwitchPlayerChangesPlayer) {
    Color initialPlayer = game->getCurrentPlayer();
    game->switchPlayer();
    EXPECT_NE(game->getCurrentPlayer(), initialPlayer);
}