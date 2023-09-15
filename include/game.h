#pragma once
#include "board.h"
#include "board_rules.h"
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
    Game() : whitePlayer(), blackPlayer(), pPreviousMove_(nullptr) {};
    Game(Player& player_1, Player& player_2);
    virtual ~Game() = default;

    virtual void startGame();
    virtual void endGame();
    virtual void movePiece(const Move& move);
    virtual bool isGameOver() const;
    virtual void switchPlayer();

    virtual GameState getGameState() const {return gameState_;}
    virtual Player getCurrentPlayer() const {return currentPlayer_;}
    virtual Board getBoard() const {return board_;}

private:
    std::set<IPiece*> pPieces_;

    Player currentPlayer_;
    Player whitePlayer;
    Player blackPlayer;

    Move* pPreviousMove_;

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