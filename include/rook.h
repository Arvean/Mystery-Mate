#pragma once

#include "piece.h"

class Rook : public IPiece {
    public:
        Rook() : id_(0), type_(PieceType::ROOK), color_(Color::WHITE) {};
        Rook(int id, Color color) : id_(id), hasMoved_(false), type_(PieceType::ROOK), color_(color) {};
        Rook(const Rook& other) : id_(other.id_), type_(other.type_), color_(other.color_) {}

        virtual IPiece* clone() const override {return new Rook(*this);} // Copy constructor is invoked here}
        virtual bool isValidMove(const Move& move) const;

        virtual std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        virtual void setHasMoved() {hasMoved_ = true;};
        virtual bool getHasMoved() const {return hasMoved_;}
        virtual PieceType getType() const {return type_;}
        virtual Color getColor() const {return color_;}
        virtual int getID() const {return id_;}

    private:
        const int id_;
        bool hasMoved_;
        const PieceType type_;
        const Color color_;
};