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
using ::testing::SetArgPointee;
using ::testing::Eq;
using ::testing::AnyNumber;
using ::testing::Invoke;

class GameTest : public ::testing::Test {
protected:
    MockPlayer *mockPlayer1, *mockPlayer2;
    MockBoard *mockBoard;
    MockBoardRules *mockRules;
    MockSquare *mockSquare;
    MockPiece *mockPiece;

    static const int VALID_HORCRUXE_ID = 1; 

    void SetUp() override {
        mockPlayer1 = new MockPlayer(Color::WHITE);
        mockPlayer2 = new MockPlayer(Color::BLACK);
        mockSquare = new MockSquare();
        mockBoard = new MockBoard();
        mockRules = new MockBoardRules();
        mockPiece = new MockPiece();


        ON_CALL(*mockBoard, getSquare(_))
        .WillByDefault(Return(mockSquare));

        ON_CALL(*mockSquare, getPiece())
        .WillByDefault(Return(mockPiece));

        ON_CALL(*mockPlayer1, getColor())
        .WillByDefault(Return(Color::WHITE));

        ON_CALL(*mockPlayer2, getColor())
        .WillByDefault(Return(Color::BLACK));

        ON_CALL(*mockPlayer1, getHorcruxID())
        .WillByDefault(Return(1));

        ON_CALL(*mockPlayer2, getHorcruxID())
        .WillByDefault(Return(1));

        //game = new Game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    }

    void TearDown() override {
        delete mockPlayer1;
        delete mockPlayer2;
        delete mockBoard;
        delete mockRules;
        delete mockPiece;
        delete mockSquare;
    }
};

TEST_F(GameTest, Constructor_ThrowsException_IfPlayersAreNull) {
    EXPECT_THROW({
        Game game(nullptr, nullptr, mockBoard, mockRules);
    }, std::logic_error);
}

TEST_F(GameTest, Constructor_InitializesWithTwoPlayers) {
    EXPECT_CALL(*mockPlayer1, getColor())
        .WillOnce(Return(Color::WHITE));
    EXPECT_CALL(*mockPlayer2, getColor())
        .WillOnce(Return(Color::BLACK));

    EXPECT_NO_THROW({
        Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    });
}

TEST_F(GameTest, CheckHorcruxSet_ReturnsTrue_IfBothPlayersSet) {
    EXPECT_CALL(*mockPlayer1, getHorcruxID())
        .WillOnce(Return(VALID_HORCRUXE_ID));
    EXPECT_CALL(*mockPlayer2, getHorcruxID())
        .WillOnce(Return(VALID_HORCRUXE_ID));
    
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();
    EXPECT_TRUE(game.checkHorcruxSet());
}

// Inside your test case
TEST_F(GameTest, MovePiece_ExecutesSuccessfully_WhenMoveIsValid) {
    // Initialize positions and move
    const Position startPos('d', 2);
    const Position endPos('d', 4);
    Move validMove(mockPiece, startPos, endPos);

    // Create mock squares
    MockSquare startSquare;
    MockSquare endSquare;

    // Setup the behavior of the 'isOccupied' method to indicate that the startSquare is occupied.
    EXPECT_CALL(startSquare, isOccupied())
        .WillRepeatedly(Return(true));

    EXPECT_CALL(startSquare, getPiece())
        .WillRepeatedly(Return(static_cast<const IPiece*>(mockPiece)));
    EXPECT_CALL(endSquare, getPiece())
        .WillRepeatedly(Return(nullptr)); // The end square is initially unoccupied

    // Setup mock board to return our mock squares
    EXPECT_CALL(*mockBoard, getSquare(startPos))
        .WillRepeatedly(Return(&startSquare));
    EXPECT_CALL(*mockBoard, getSquare(endPos))
        .WillRepeatedly(Return(&endSquare));

    // Simulate the rules saying that the move is valid
    EXPECT_CALL(*mockRules, isValidMove(_, _, _))
        .WillRepeatedly(Return(true));

    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    // Verify that the piece was removed from the original square and placed on the new square
    EXPECT_CALL(startSquare, removePiece())
        .Times(1);
    EXPECT_CALL(endSquare, placePiece(mockPiece))
        .Times(1);

    // Attempt the move
    EXPECT_NO_THROW({
        game.movePiece(validMove, mockPlayer1);
    });
}

// Test for handling an invalid move
TEST_F(GameTest, MovePiece_ThrowsException_WhenMoveIsInvalid) {
    Move invalidMove(mockPiece, Position('d', 4), Position('d', 2));

    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    EXPECT_THROW({
        game.movePiece(invalidMove, mockPlayer1);
    }, std::logic_error);
}

// Test checkGameOver method when the game is not over
TEST_F(GameTest, CheckGameOver_ReturnsFalse_WhenGameIsNotOver) {
    // Mock the necessary methods to simulate a game that is not over.
    // For instance, no checkmate, no stalemate, etc.

    // Mocking the horcrux capture condition, if it exists.
    ON_CALL(*mockPlayer1, getHorcruxID())
        .WillByDefault(Return(VALID_HORCRUXE_ID));  // Let's assume '1' means no horcrux captured
    ON_CALL(*mockPlayer2, getHorcruxID())
        .WillByDefault(Return(VALID_HORCRUXE_ID));

    EXPECT_CALL(*mockRules, isValidMove(_, _, _))
        .WillRepeatedly(Return(true));

    // Now, you create your game object.
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    mockPlayer1->setHorcruxID(3);
    mockPlayer2->setHorcruxID(19);

    // When the game state is such that the game isn't over, 
    // "checkGameOver" should return false
    EXPECT_FALSE(game.checkGameOver());
}

// Test case for checkmate scenario
TEST_F(GameTest, CheckGameOver_ReturnsTrue_WhenCheckmate) {
    // Setup mock behavior. In a checkmate scenario, the king is in check, and there are no valid moves.
    Position kingPosition('e', 1);
    EXPECT_CALL(*mockBoard, findKing(_)).WillRepeatedly(Return(&kingPosition));  // King is assumed to be found.

    EXPECT_CALL(*mockRules, isInCheck(_, Color::WHITE)).WillRepeatedly(Return(true));  // King is in check.

    // No valid moves for any piece, signifying checkmate.
    EXPECT_CALL(*mockRules, isValidMove(_, _, _)).WillRepeatedly(Return(false));

    // Construct game object with the mock dependencies. This assumes the Game class has a constructor that accepts these dependencies.
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    // Call the function to test. We expect checkGameOver to return true, indicating a checkmate has occurred.
    bool isGameOver = game.checkGameOver();
    EXPECT_TRUE(isGameOver);

    // Further checks could be made here depending on the functionality of checkGameOver.
    // For example, if the game state changes or specific players are notified, etc.
}
/*
// Test case for checkmate scenario (we already discussed this one)
TEST_F(GameTest, CheckGameOver_WhenCheckmate) {
    MockGame game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    EXPECT_CALL(game, _isCheckmate(_))
        .WillOnce(Return(true));

    bool isGameOver = game.checkGameOver();
    EXPECT_TRUE(isGameOver);
    EXPECT_EQ(game.getGameResult(), GameEndType::BLACK_WIN); // Assuming it's white's turn and it got checkmated.
}
*/

// Test case for horcrux capture scenario
TEST_F(GameTest, CheckGameOver_WhenHorcruxCaptured) {
    int WHITE_HORCRUX_ID = 3;

    ON_CALL(*mockPlayer1, getHorcruxID())
        .WillByDefault(Return(WHITE_HORCRUX_ID));  // Let's assume '1' means no horcrux captured

    Board realBoard;
    MockGame game(mockPlayer1, mockPlayer2, &realBoard, mockRules);
    game.startGame();

    realBoard.removePiece(realBoard.findSquare(WHITE_HORCRUX_ID));

    bool isGameOver = game.checkGameOver();
    EXPECT_TRUE(isGameOver);
    EXPECT_EQ(game.getGameResult(), GameEndType::BLACK_WIN); // Assuming white's horcrux was captured.
}

// Test case for stalemate scenario
TEST_F(GameTest, CheckGameOver_WhenStalemate) {
    MockGame game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    EXPECT_CALL(game, _isStalemate())
        .WillOnce(Return(true));

    bool isGameOver = game.checkGameOver();
    EXPECT_TRUE(isGameOver);
    EXPECT_EQ(game.getGameResult(), GameEndType::STALEMATE);
}

// Test case for insufficient material scenario
TEST_F(GameTest, CheckGameOver_WhenInsufficientMaterial) {
    MockGame game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    EXPECT_CALL(game, _isStalemate())
    .Times(AnyNumber())  // or .Times(0) if it shouldn't be called
    .WillRepeatedly(Return(false)); // Ensuring that any call to this returns false

    EXPECT_CALL(game, _hasInsufficientMaterial())
        .WillOnce(Return(true));

    bool isGameOver = game.checkGameOver();
    EXPECT_TRUE(isGameOver);
    EXPECT_EQ(game.getGameResult(), GameEndType::DRAW);
}

// You can add more tests for other scenarios...
// Test the horcruxGuess method
TEST_F(GameTest, HorcruxGuess_ReturnsTrue_WhenGuessIsCorrect) {
    int correctHorcruxID = VALID_HORCRUXE_ID;
    EXPECT_CALL(*mockPlayer1, getHorcruxID())
        .WillRepeatedly(Return(correctHorcruxID));

    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();
    EXPECT_TRUE(game.horcruxGuess(correctHorcruxID, mockPlayer1));
}

TEST_F(GameTest, HorcruxGuess_ReturnsFalse_WhenGuessIsIncorrect) {
    int incorrectHorcruxID = 8;
    EXPECT_CALL(*mockPlayer1, getHorcruxID())
        .WillRepeatedly(Return(VALID_HORCRUXE_ID));

    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();
    EXPECT_FALSE(game.horcruxGuess(incorrectHorcruxID, mockPlayer1));
}

// Test to check if the game state is retrieved correctly
TEST_F(GameTest, GetGameState_ReturnsCorrectState) {
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();  // Assuming this changes the state

    // Check if the state after starting the game is as expected
    EXPECT_EQ(game.getGameState(), GameState::CHOOSING_HORCRUX); // or another appropriate state
}

// Test to check the current player is retrieved correctly
TEST_F(GameTest, GetCurrentPlayer_ReturnsCorrectPlayer) {
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    // Depending on whose turn it is supposed to be first (assuming mockPlayer1 here)
    EXPECT_EQ(game.getCurrentPlayer(), mockPlayer1);
}

// Test to check if the correct game board is returned
TEST_F(GameTest, GetBoard_ReturnsActualBoard) {
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    // Check if the correct board object is returned
    EXPECT_EQ(game.getBoard(), mockBoard);
}

// Test for the situation where the horcrux guess is correct
TEST_F(GameTest, HorcruxGuess_SuccessfulWhenCorrect) {
    int correctHorcruxID = 1; // Assuming 1 is correct for testing
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    EXPECT_TRUE(game.horcruxGuess(correctHorcruxID, mockPlayer1));
}

// Test for retrieving available positions for a piece
TEST_F(GameTest, GetAvailablePositions_ReturnsValidPositions) {
    Game game(mockPlayer1, mockPlayer2, mockBoard, mockRules);
    game.startGame();

    Position pos('e', 2);
    Move previousPos(mockPiece, Position('f', 2), Position('f', 3));
    
    // Define the expected positions
    std::unordered_set<Position> expectedPositions = {
        Position('e', 4), Position('e', 5) // Just examples, use actual board positions
    };

    // Set expectation that `generateValidPositions` will be called with our piece and should return the `expectedPositions`
    EXPECT_CALL(*mockRules, generateValidPositions(_, mockPiece, pos, previousPos))
        .WillOnce(Return(expectedPositions));

    // Call the function to test
    auto actualPositions = game.getAvailablePositions(mockPiece, pos);

    // Validate the positions received are as expected
    EXPECT_EQ(actualPositions, expectedPositions);
}