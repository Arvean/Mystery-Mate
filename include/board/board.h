#pragma once

#include "square.h"
#include "piece.h"
#include <unordered_map>

class Board {
    public:
        Board();
        virtual ~Board() = default;

        void placePiece(const Position& position, Piece* piece); // Remove?
        Piece* getPiece(const Position& position) const; // Remove?
        bool isValidMove(const Move& move) const;
        void movePiece(const Move& move);

    private:
        std::unordered_map<Position, Square> squares_;

        void takePiece_(const Square& square);
        //bool checkPromotion_(Piece* piece, Position& position) {
        //void promotion_(Piece* piece, Position& position) {

        // Promotion is a rule and should be apart of the game class
};