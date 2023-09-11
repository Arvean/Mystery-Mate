#pragma once

#include "piece.h"

class Bishop : public IPiece {
    public:
        Bishop() : id_(0), type_(PieceType::BISHOP), color_(Color::WHITE) {};
        Bishop(int id, Color color) : id_(id), type_(PieceType::BISHOP), color_(color) {};
        Bishop(const Bishop& other) : id_(other.id_), type_(other.type_), color_(other.color_) {}

        IPiece* clone() const override {return new Bishop(*this);} // Copy constructor is invoked here}

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