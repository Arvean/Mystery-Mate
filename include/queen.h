#pragma once

#include "piece.h"

class Queen : public IPiece {
    public:
        Queen() : id_(0), type_(PieceType::QUEEN), color_(Color::WHITE) {};
        Queen(int id, Color color) : id_(id), type_(PieceType::QUEEN), color_(color) {};
        Queen(const Queen& other) : id_(other.id_), type_(other.type_), color_(other.color_) {}

        IPiece* clone() const override {return new Queen(*this);} // Copy constructor is invoked here}

        bool isValidMove(const Move& move) const;
        std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        PieceType getType() const {return type_;}
        Color getColor() const {return color_;}
        int getID() const {return id_;}

    private:
        const int id_;
        const PieceType type_;
        const Color color_;
};