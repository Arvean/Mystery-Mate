#pragma once

#include "piece.h"

class Pawn : public IPiece {
    public:
        Pawn() : id_(0), type_(PieceType::PAWN), color_(Color::WHITE) {};
        Pawn(int id, Color color) : id_(id), type_(PieceType::PAWN), color_(color) {};
        Pawn(const Pawn& other) : id_(other.id_), type_(other.type_), color_(other.color_) {}

        virtual IPiece* clone() const override {return new Pawn(*this);} // Copy constructor is invoked here}
        
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