#pragma once
#include "board.h"
#include "board_rules.h"
#include "move.h"
#include "player.h"
#include <set>

enum class GameState {
    WAITING_FOR_OPPONENT,
    WHITE_MOVE,
    BLACK_MOVE,
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
    Game() : whitePlayer(), blackPlayer(), pPreviousMove_(nullptr),
             board_(new Board()), boardRules_(new BoardRules()) {};
    Game(Player& player_1, Player& player_2, Board* board, BoardRules* boardRules);
    virtual ~Game() = default;

    virtual void startGame();
    virtual void movePiece(const Move& move);
    virtual bool isGameOver();

    virtual GameState getGameState() const {return gameState_;}
    virtual const Player* getCurrentPlayer() const {return pCurrentPlayer_;}
    virtual Board* getBoard() const {return board_;}
    virtual GameEndType getGameResult();

private:
    std::set<IPiece*> pPieces_;

    const Player* pCurrentPlayer_;
    Player whitePlayer;
    Player blackPlayer;

    Move* pPreviousMove_;

    Board* board_;
    BoardRules* boardRules_;
    GameState gameState_;
    GameEndType gameEndType_;

    void _endGame();
    void _switchPlayer();
    void _setupBoard();
    void _updateGameState();
    virtual bool _isHorcruxeCaptured(const int horcruxeID) const;
    virtual bool _isStalemate() const;
    virtual bool _hasInsufficientMaterial() const;
    //void _isThreefoldRepetition() const;
    //void _isFifyMoveRule() const;
    //std::vector<MoveRecord> moveHistory_;

};