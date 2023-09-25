#include "game.h"
#include "board.h"
#include "board_rules.h"
#include "king.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "pawn.h"
#include "rook.h"
#include <array>
#include <iostream>


Game::Game(Player* player_1, Player* player_2, 
           Board* board, BoardRules* boardRules)
    : whitePlayer(player_1), blackPlayer(player_2), 
      board_(board), boardRules_(boardRules), 
      pPreviousMove_(nullptr) 
{
    if (player_1->getColor() == Color::WHITE && player_2->getColor() == Color::BLACK) {
        whitePlayer = player_1;
        blackPlayer = player_2;
    } else if ((player_1->getColor() == Color::BLACK && player_2->getColor() == Color::WHITE)) {
        whitePlayer = player_2;
        blackPlayer = player_1;
    } else {
        throw std::logic_error("Require one white and one black player");
    }
}


void Game::_setupBoard() {
    int whiteID = MIN_WHITE_HORCRUXE_ID;
    int blackID = MIN_BLACK_HORCRUXE_ID;

    auto placeAndStore = [this](const Position& pos, IPiece* piece) {
        board_->placePiece(pos, piece);
        pieceMap[piece->getID()] = piece;
    };

    // Pawns
    for (char file = 'a'; file <= 'h'; ++file) {
        placeAndStore(Position(file, 2), new Pawn(whiteID++, Color::WHITE));
        placeAndStore(Position(file, 7), new Pawn(blackID++, Color::BLACK));
    }

    // Rooks
    placeAndStore(Position('a', 1), new Rook(whiteID++, Color::WHITE));
    placeAndStore(Position('h', 1), new Rook(whiteID++, Color::WHITE));
    placeAndStore(Position('a', 8), new Rook(blackID++, Color::BLACK));
    placeAndStore(Position('h', 8), new Rook(blackID++, Color::BLACK));

    // Knights
    placeAndStore(Position('b', 1), new Knight(whiteID++, Color::WHITE));
    placeAndStore(Position('g', 1), new Knight(whiteID++, Color::WHITE));
    placeAndStore(Position('b', 8), new Knight(blackID++, Color::BLACK));
    placeAndStore(Position('g', 8), new Knight(blackID++, Color::BLACK));

    // Bishops
    placeAndStore(Position('c', 1), new Bishop(whiteID++, Color::WHITE));
    placeAndStore(Position('f', 1), new Bishop(whiteID++, Color::WHITE));
    placeAndStore(Position('c', 8), new Bishop(blackID++, Color::BLACK));
    placeAndStore(Position('f', 8), new Bishop(blackID++, Color::BLACK));

    // Queens
    placeAndStore(Position('d', 1), new Queen(whiteID++, Color::WHITE));
    placeAndStore(Position('d', 8), new Queen(blackID++, Color::BLACK));

    // Kings
    placeAndStore(Position('e', 1), new King(whiteID++, Color::WHITE));
    placeAndStore(Position('e', 8), new King(blackID++, Color::BLACK));
};



void Game::startGame() {
    pCurrentPlayer_ = whitePlayer; // Current player white
    gameState_ = GameState::WHITE_MOVE;

    _setupBoard();
};


void Game::movePiece(const Move& move, const Player* pPlayer) {
    Square* pSquareFrom = board_->getSquare(move.getFrom());
    if (pSquareFrom->isOccupied()) {
        if (pPlayer->getColor() != pSquareFrom->getPiece()->getColor()) {
            throw std::logic_error("Invalid move. Player color does not match piece color");
        }
    } else {throw std::logic_error("The current move is not valid. From Square not occupied.");}

    Square* pSquareTo = board_->getSquare(move.getTo());
    std::cout << "Returned Square pointer: " << pSquareTo << std::endl;

    if (pSquareTo == nullptr) {throw std::logic_error("Invalid move. Not a valid square");}

    if (boardRules_->isValidMove(*board_, move, pPreviousMove_)) {
        if (pSquareTo->isOccupied()) {
            IPiece* capturedPiece = const_cast<IPiece*> (pSquareTo->getPiece());
            if (pieceMap.find(capturedPiece->getID()) != pieceMap.end()) {
                pieceMap.erase(capturedPiece->getID());
                delete capturedPiece; // Due to object being dynamically allocated
            } else {throw std::logic_error("Piece does not exit");}
        } else {pSquareTo->placePiece(const_cast<const IPiece*> (move.getPiece()));};
        if (checkGameOver()) {
            _endGame();
        } else {
            pPreviousMove_ = const_cast<Move*> (&move);
            _switchPlayer();
        }
    } else {throw std::logic_error("Invalid Move. Please try another move.");}
};


std::unordered_set<Position> Game::getAvailablePositions(IPiece* piece) {
    if (piece) {
        boardRules_->generateValidPositions(*board_, piece);
    } else {throw std::logic_error("Piece is nullptr. No available moves.");}
}


void Game::horcruxeGuess(const int horcruxeID, const Player* pPlayer) {
    if (_isHorcruxeGuessed(horcruxeID, pPlayer)) {
        if (pPlayer->getColor() == Color::WHITE) {gameEndType_ = GameEndType::BLACK_WIN;}
        else {gameEndType_ = GameEndType::WHITE_WIN;}
        _endGame();
    }
}


bool Game::checkGameOver() {
    if (gameState_ == GameState::ENDED) {
        return true;
    }

    if (_isHorcruxeCaptured(whitePlayer->getHorcruxeID())) {
        _endGame();
        gameEndType_ = GameEndType::BLACK_WIN;
        return true;
    }
    if (_isHorcruxeCaptured(blackPlayer->getHorcruxeID())) {
        _endGame();
        gameEndType_ = GameEndType::WHITE_WIN;
        return true;
    }
    if (_isStalemate()) {
        _endGame();
        gameEndType_ = GameEndType::STALEMATE;
        return true;
    }
    if (_hasInsufficientMaterial()) {
        _endGame();
        gameEndType_ = GameEndType::DRAW;
        return true;
    }
    return false;
};


GameEndType Game::getGameResult() {
    if (gameState_ == GameState::ENDED) {return gameEndType_;}
}; 


void Game::_endGame() {
    gameState_ = GameState::ENDED;
}; 


void Game::_switchPlayer() {
    pCurrentPlayer_->getColor() == Color::WHITE ? pCurrentPlayer_ = blackPlayer : pCurrentPlayer_ = whitePlayer;
    _updateGameState();
}


void Game::_updateGameState() {
    pCurrentPlayer_->getColor() == Color::WHITE ? gameState_ = GameState::WHITE_MOVE : gameState_ = GameState::BLACK_MOVE;
}


bool Game::_isHorcruxeGuessed(const int horcruxeID, const Player* pPlayer) const {
    if (pieceMap.find(horcruxeID) == pieceMap.end()) {
        throw std::logic_error("Invalid horcruxe ID");
    }
    if (pPlayer->getHorcruxeID() == horcruxeID) {
        return true;
    }
    return false;
};


bool Game::_isHorcruxeCaptured(const int horcruxeID) const {
    for (const auto& pair : board_->squares) {
        const Square* currentSquare = pair.second.get();

        if (currentSquare && currentSquare->getPiece() && currentSquare->getPiece()->getID() == horcruxeID) {return false;}
    }
    return true;
};


bool Game::_isStalemate() const {
    std::array<Color, 2> colors = { Color::WHITE, Color::BLACK };

    for (const auto& playerColor : colors) {
        if (boardRules_->isInCheck(*board_, playerColor)) {return false;}

        for (const auto& it : pieceMap) {
            if (it.second->getColor() == playerColor) {
                Position* pPiecePosition;
                for (const auto& pair : board_->squares) {
                    if (pair.second->getPiece()->getID() == it.first) {
                        pPiecePosition = const_cast<Position*> (&pair.first);
                        break;
                    }
                }
                auto possiblePos = it.second->getPossiblePositions(*pPiecePosition);
                for (const Position& pos : possiblePos) {
                    if (boardRules_->isValidMove(*board_, Move(it.second, *pPiecePosition, pos), pPreviousMove_)) {return false;}
                }
            }
        }
    }
    return true;
};


bool Game::_hasInsufficientMaterial() const {
    int numBishopsLightSquare = 0;
    int numBishopsDarkSquare = 0;
    int numKnights = 0;
    int numPawns = 0;
    int numRooks = 0;
    int numQueens = 0;

    // Loop through all squares on the board to count the pieces.
    for (const auto& pair : board_->squares) {
        const Square* square = pair.second.get();
        if (square && square->getPiece()) {
            switch (square->getPiece()->getType()) {
                case PieceType::BISHOP:
                    if (Board::isLightSquare(square->getPosition())) {
                        numBishopsLightSquare++;
                    } else {
                        numBishopsDarkSquare++;
                    }
                    break;
                case PieceType::KNIGHT:
                    numKnights++;
                    break;
                case PieceType::PAWN:
                    numPawns++;
                    break;
                case PieceType::ROOK:
                    numRooks++;
                    break;
                case PieceType::QUEEN:
                    numQueens++;
                    break;
                default:
                    break;
            }
        }
    }

    // Check the known scenarios for insufficient material:
    if (numPawns == 0 && numRooks == 0 && numQueens == 0) {
        if (numKnights <= 1 && (numBishopsLightSquare + numBishopsDarkSquare == 0)) {
            return true;
        }
        if (numKnights == 0 && (numBishopsLightSquare == 1 || numBishopsDarkSquare == 1)) {
            return true;
        }
        if (numBishopsLightSquare == 1 && numBishopsDarkSquare == 1) {
            return true;
        }
    }
    return false;
}
