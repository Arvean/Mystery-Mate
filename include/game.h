#pragma once
#include "board.h"
#include "board_rules.h"
#include "move.h"
#include "player.h"
#include <set>

enum class GameState {
    WAITING_FOR_OPPONENT,
    CHOOSING_HORCRUX,
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
    Game() : whitePlayer(), blackPlayer(), previousMove_(Move()),
             board_(new Board()), boardRules_(new BoardRules()) {};
    Game(Player* player_1, Player* player_2, Board* board, BoardRules* boardRules);
    virtual ~Game() {  // Destructor of your Game class
        _cleanupPieces();
    }


    virtual void startGame();
    virtual void movePiece(const Move& move, Player* pPlayer);
    virtual bool checkGameOver();

    virtual GameState getGameState() const {return gameState_;}
    virtual const Player* getCurrentPlayer() const {return pCurrentPlayer_;}
    virtual Board* getBoard() const {return board_;}
    virtual GameEndType getGameResult();
    virtual std::unordered_set<Position> getAvailablePositions(IPiece* piece, const Position& from);
    virtual const IPiece* getPieceFromID(int id) {return pieceMap.at(id);}
    virtual const IPiece* getPieceFromPosition(const Position& position) {
        return board_->getSquare(position)->getPiece();
    }
    virtual bool isKingCaptured(Color kingColor) const;

    virtual bool horcruxGuess(const int guessedHorcruxID, Player* guessingPlayer, Player* playerToCheck);
    virtual bool checkHorcruxSet();

    Player* whitePlayer;
    Player* blackPlayer;

protected:
    virtual void _validateMoveForPlayer(const Square* pSquareFrom, const Player* pPlayer) const;
    virtual void _executeMove(Square* pSquareFrom, Square* pSquareTo, const Move& move);
    virtual void _endGame();
    virtual void _switchPlayer();
    virtual void _setupBoard();
    virtual void _updateGameState();
    virtual bool _isHorcruxGuessed(const int horcruxID, const Player* pPlayer) const;
    virtual bool _isHorcruxCaptured(const int horcruxID) const;
    virtual bool _isStalemate() const;
    virtual bool _hasInsufficientMaterial() const;
    //void _isThreefoldRepetition() const;
    //void _isFifyMoveRule() const;
    //std::vector<MoveRecord> moveHistory_;

    void _cleanupPieces() {
        for (auto& id_piece_pair : pieceMap) {
            delete id_piece_pair.second;  // delete the piece, which was allocated with new
            id_piece_pair.second = nullptr;  // set the pointer to nullptr, preventing any future access
        }
        pieceMap.clear();  // removes all elements from the map
    }


private:
    std::unordered_map<int, IPiece*> pieceMap;

    const Player* pCurrentPlayer_;

    Move previousMove_;
    Board* board_;
    BoardRules* boardRules_;
    GameState gameState_;
    GameEndType gameEndType_;
};