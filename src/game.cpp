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
      previousMove_(Move()) 
{
    if (!player_1 || !player_2) {
        throw std::logic_error("Players cannot be null");
    }
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
    gameState_ = GameState::CHOOSING_HORCRUX;

    _setupBoard();
};

bool Game::checkHorcruxSet() {
    if (whitePlayer->getHorcruxID() && blackPlayer->getHorcruxID()) {
        gameState_ = GameState::WHITE_MOVE;
        return true;
    }
    return false;
};

// Validate the move based on the player's color and piece occupancy
void Game::_validateMoveForPlayer(const Square* pSquareFrom, const Player* pPlayer) const {
    if (!pSquareFrom->isOccupied()) {
        throw std::logic_error("The current move is not valid. From Square not occupied.");
    }

    if (pPlayer->getColor() != pSquareFrom->getPiece()->getColor()) {
        throw std::logic_error("Invalid move. Player color does not match piece color");
    }
}

// Execute the move and handle the captured piece if present
void Game::_executeMove(Square* pSquareFrom, Square* pSquareTo, const Move& move) {
    if (pSquareTo->isOccupied()) {
        pSquareTo->removePiece();
    }
    pSquareTo->placePiece(move.getPiece());
    pSquareFrom->removePiece();

    // Check for castling (king's move of two squares to the right or left)
    const IPiece* movedPiece = move.getPiece();
    if (movedPiece->getType() == PieceType::KING) {
        int moveDistance = pSquareTo->getPosition().getFile() - pSquareFrom->getPosition().getFile();
        if (std::abs(moveDistance) == 2) {
            // This is a castling move
            Square* rookSquareFrom;
            Square* rookSquareTo;

            if (moveDistance > 0) {
                // King-side castling
                rookSquareFrom = board_->getSquare(Position('h', pSquareFrom->getPosition().getRank()));
                rookSquareTo = board_->getSquare(Position('f', pSquareFrom->getPosition().getRank()));
            } else {
                // Queen-side castling
                rookSquareFrom = board_->getSquare(Position('a', pSquareFrom->getPosition().getRank()));
                rookSquareTo = board_->getSquare(Position('d', pSquareFrom->getPosition().getRank()));
            }

            // Move the rook
            rookSquareTo->placePiece(rookSquareFrom->getPiece());
            rookSquareFrom->removePiece();
        }
    }
}

// Main function that uses the helper functions
void Game::movePiece(const Move& move, Player* pPlayer) {
    Square* pSquareFrom = board_->getSquare(move.getFrom());
    _validateMoveForPlayer(pSquareFrom, pPlayer);

    Square* pSquareTo = board_->getSquare(move.getTo());
    if (pSquareTo == nullptr) {
        throw std::logic_error("Invalid move. Not a valid square");
    }

    if (!boardRules_->isValidMove(*board_, move, previousMove_)) {
        throw std::logic_error("Invalid Move. Please try another move.");
    }

    _executeMove(pSquareFrom, pSquareTo, move);

    if (isKingCaptured(pPlayer->getColor())) {
        pPlayer->setHasKingBeenCaptured();
    }

    if (checkGameOver()) {
        _endGame();
    } else {
        previousMove_ = move;
        _switchPlayer();
    }
}

std::unordered_set<Position> Game::getAvailablePositions(IPiece* piece, const Position& from) {
    if (piece) {
        return boardRules_->generateValidPositions(*board_, piece, from, previousMove_);
    } else {throw std::logic_error("Piece is nullptr. No available moves.");}
}


bool Game::horcruxGuess(const int guessedHorcruxID, Player* pPlayer) {
    if (pPlayer->getHorcruxFound()) {
        throw std::logic_error("Horcrux is already guessed");
        return false;
    }
    if (pieceMap.find(guessedHorcruxID) == pieceMap.end()) {
        throw std::logic_error("Invalid horcrux ID");
        return false;
    }

    if (pPlayer->getNumberOfHorcruxGuessesLeft() <= 0) {
        throw std::logic_error("No more Horcrux guesses left");
        return false;
    };

    pPlayer->decrementNumberOfHorcruxGuessesleft();

    if (_isHorcruxGuessed(guessedHorcruxID, pPlayer)) {
        pPlayer->setHorcruxFound();
        return true;
    }
    return false;
}


bool Game::checkGameOver() {
    if (gameState_ == GameState::ENDED) {
        return true;
    }

    Player* player = const_cast<Player*>(getCurrentPlayer());

    if (_isHorcruxCaptured(player->getHorcruxID())) {
        player->getColor() == Color::WHITE ? gameEndType_ = GameEndType::BLACK_WIN
                                          : gameEndType_ = GameEndType::WHITE_WIN;
        player->setHasHorcruxBeenCaptured();
        return true;
    }

    if (_isStalemate()) {
        gameEndType_ = GameEndType::STALEMATE;
        return true;
    }
    if (_hasInsufficientMaterial()) {
        gameEndType_ = GameEndType::DRAW;
        return true;
    }
    return false;
};


GameEndType Game::getGameResult() {
    if (gameState_ == GameState::ENDED) {return gameEndType_;}
    else {throw std::logic_error("Game is not complete");}
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

bool Game::isKingCaptured(Color kingColor) const {
    return !board_->findKing(kingColor) ? true : false;
}

bool Game::_isHorcruxGuessed(const int horcruxID, const Player* pPlayer) const {
    if (pPlayer->getHorcruxID() == horcruxID) {
        return true;
    }
    return false;
};


bool Game::_isHorcruxCaptured(const int horcruxID) const {
    for (const auto& pair : board_->squares) {
        const Square* currentSquare = pair.second.get();

        if (currentSquare && currentSquare->getPiece() && currentSquare->getPiece()->getID() == horcruxID) {return false;}
    }
    return true;
};


bool Game::_isStalemate() const {
    std::array<Color, 2> colors = { Color::WHITE, Color::BLACK };

    for (const auto& playerColor : colors) {
        if (boardRules_->isInCheck(*board_, playerColor)) {return false;}

        for (const auto& it : pieceMap) {
            if (it.second->getColor() == playerColor) {
                const Position* pPiecePosition;
                for (const auto& pair : board_->squares) {
                    const IPiece* piece = pair.second->getPiece();  // Getting the piece
                    if (piece != nullptr && piece->getID() == it.first) {  // Check if piece is not nullptr before accessing it
                        pPiecePosition = &pair.first;
                        break;
                    }
                }
                auto possiblePos = it.second->getPossiblePositions(*pPiecePosition);
                for (const Position& pos : possiblePos) {
                    if (boardRules_->isValidMove(*board_, Move(it.second, *pPiecePosition, pos), previousMove_)) {return false;}
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
