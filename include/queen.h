#pragma once

#include "piece.h"

class Queen : public IPiece {
    public:
        Queen() : id_(0), type_(PieceType::QUEEN), color_(Color::WHITE) {};
        Queen(int id, Color color) : id_(id), type_(PieceType::QUEEN), color_(color) {};
        Queen(const Queen& other) : id_(other.id_), type_(other.type_), color_(other.color_) {}

        virtual IPiece* clone() const override {return new Queen(*this);} // Copy constructor is invoked here}

        virtual bool isValidMove(const Move& move) const;
        virtual std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        virtual PieceType getType() const {return type_;}
        virtual Color getColor() const {return color_;}
        virtual int getID() const {return id_;}

    private:
        const int id_;
        const PieceType type_;
        const Color color_;
};