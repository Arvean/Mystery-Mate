#pragma once

#include "piece.h"

class MockPiece : public IPiece {
public:
    MockPiece() : symbol_(' '), color_(Color::WHITE) {};
    MockPiece(Color color) : symbol_(' '), color_(color) {};
    ~MockPiece() override = default;

    bool isValidMove(const Move& move) const override {
        return false; // Returning a default value
    }

    std::unordered_set<Position> getPossiblePositions(const Position& /*from*/) const override {
        return {}; // Returning an empty set
    }

    char getSymbol() const override {
        return symbol_; // Returning a mock symbol
    }

    Color getColor() const override {
        return color_; // Returning a default color
    }

private:
    const char symbol_;
    const Color color_;
};
