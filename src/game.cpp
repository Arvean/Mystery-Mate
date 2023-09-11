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

#define NUMBER_OF_PIECES 32U


Game::Game(Player& player_1, Player& player_2) {
    if (player_1.getColor() == Color::WHITE && player_2.getColor() == Color::BLACK) {
        whitePlayer = player_1;
        blackPlayer = player_2;
    } else if ((player_1.getColor() == Color::BLACK && player_2.getColor() == Color::WHITE)) {
        whitePlayer = player_2;
        blackPlayer = player_1;
    } else {
        throw std::logic_error("Require one white and one black player");
    }
}


void Game::_setupBoard() {
    int whiteID = 1;
    int blackID = 17;

    auto placeAndStore = [this](const Position& pos, IPiece* piece) {
        board_.placePiece(pos, piece);
        pPieces_.insert(piece);
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
    currentPlayer_ = whitePlayer; // Current player white
    gameState_ = GameState::IN_PROGRESS;

    _setupBoard();
};


void Game::endGame() {
    gameState_ = GameState::ENDED;
}; // Note to handle edge case of game never ending


void Game::movePiece(const Move& move) {
    Square* pSquare = board_.getSquare(move.getTo());
    if (pSquare == nullptr) {throw std::logic_error("Invalid move. Not a valid square");}

    if (boardRules_.isValidMove(board_, move)) {
        if (pSquare->isOccupied()) {
            IPiece* capturedPiece = const_cast<IPiece*> (pSquare->getPiece());
            if (pPieces_.find(capturedPiece) != pPieces_.end()) {
                pPieces_.erase(capturedPiece);
                delete capturedPiece; // Due to object being dynamically allocated
            } else {throw std::logic_error("Piece does not exit");}
            pSquare->placePiece(const_cast<const IPiece*> (move.getPiece()));
        } else {throw std::logic_error("Invalid move");}
    }
};


bool Game::isGameOver() const {
    if (_isHorcruxeCaptured(whitePlayer.getHorcruxeID()) || 
        _isHorcruxeCaptured(blackPlayer.getHorcruxeID()) || 
        _isStalemate() || 
        _hasInsufficientMaterial()) {
        return true;
    }
    return false;
};


void Game::switchPlayer() {
    if (currentPlayer_.getColor() == Color::WHITE) {currentPlayer_ = blackPlayer;}
    else {currentPlayer_ = whitePlayer;}
}


bool Game::_isHorcruxeCaptured(const int horcruxeID) const {
    for (const auto& pair : board_.squares) {
        const Square* currentSquare = pair.second.get();

        if (currentSquare && currentSquare->getPiece() && currentSquare->getPiece()->getID() == horcruxeID) {return false;}
    }
    return true;
};


bool Game::_isStalemate() const {
    std::array<Color, 2> colors = { Color::WHITE, Color::BLACK };

    for (const auto& playerColor : colors) {
        if (boardRules_.isInCheck(board_, playerColor)) {return false;}

        for (const auto& pPiece : pPieces_) {
            if (pPiece->getColor() == playerColor) {
                Position* pPiecePosition;
                for (const auto& pair : board_.squares) {
                    if (pair.second->getPiece() == pPiece) {
                        pPiecePosition = const_cast<Position*> (&pair.first);
                        break;
                    }
                }
                auto possiblePos = pPiece->getPossiblePositions(*pPiecePosition);
                for (const Position& pos : possiblePos) {
                    if (boardRules_.isValidMove(board_, Move(pPiece, *pPiecePosition, pos))) {return false;}
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
    for (const auto& pair : board_.squares) {
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
