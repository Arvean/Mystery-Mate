#pragma once

#include "piece.h"

class MockPiece : public IPiece {
public:
    MockPiece() : id_(0), type_(PieceType::MOCK), color_(Color::WHITE) {};
    MockPiece(int id, Color color) : id_(id), type_(PieceType::MOCK), color_(color) {};
    ~MockPiece() override = default;

    bool isValidMove(const Move& move) const override {
        return false; 
    }

    std::unordered_set<Position> getPossiblePositions(const Position& /*from*/) const override {
        return {}; 
    }

    PieceType getType() const override {
        return type_; 
    }

    Color getColor() const override {
        return color_; 
    }

    IPiece* clone() const override {return new MockPiece(*this);} // Copy constructor is invoked here}

    int getID() const override {
        return id_;
    }

private:
    const int id_;
    const PieceType type_;
    const Color color_;
};
