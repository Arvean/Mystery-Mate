#pragma once

#include "piece.h"

class King : public IPiece {
    public:
        King() : id_(0), type_(PieceType::KING), color_(Color::WHITE) {};
        King(int id, Color color) : id_(id), hasMoved_(false), type_(PieceType::KING), color_(color) {};
        King(const King& other) : id_(other.id_), type_(other.type_), color_(other.color_) {}

        IPiece* clone() const override {return new King(*this);} // Copy constructor is invoked here}

        bool isValidMove(const Move& move) const;
        std::unordered_set<Position> getPossiblePositions(const Position& from) const;
        
        void setHasMoved() {hasMoved_ = true;};
        bool getHasMoved() const {return hasMoved_;}
        PieceType getType() const {return type_;}
        Color getColor() const {return color_;}
        int getID() const {return id_;}

    private:
        const int id_;
        bool hasMoved_;
        const PieceType type_;
        const Color color_;
};