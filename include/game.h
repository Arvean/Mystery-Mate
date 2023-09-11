#pragma once
#include "board.h"
#include "move.h"
#include "player.h"
#include <set>

enum class GameState {
    IN_PROGRESS, 
    ENDED
};

enum class GameEndType {
    WHITE_WIN,   
    BLACK_WIN,      
    DRAW,           
    STALEMATE
};

class Game {
public:
    Game() : whitePlayer(), blackPlayer() {};
    Game(Player& player_1, Player& player_2);
    virtual ~Game() = default;

    void startGame();
    void endGame();
    void movePiece(const Move& move);
    bool isGameOver() const;
    void switchPlayer();

    GameState getGameState() const {return gameState_;}
    Player getCurrentPlayer() const {return currentPlayer_;}
    Board getBoard() const {return board_;}

private:
    std::set<IPiece*> pPieces_;

    Player currentPlayer_;
    Player whitePlayer;
    Player blackPlayer;

    Board board_;
    BoardRules boardRules_;
    GameState gameState_;

    void _setupBoard();
    bool _isHorcruxeCaptured(const int horcruxeID) const;
    bool _isStalemate() const;
    bool _hasInsufficientMaterial() const;
    //void _isThreefoldRepetition() const;
    //void _isFifyMoveRule() const;
    //std::vector<MoveRecord> moveHistory_;

};