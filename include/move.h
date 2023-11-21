#pragma once

#include "position.h"

class IPiece;

class Move {
public:
    Move()
        : pPiece_(nullptr), from_(Position(0, 0)), to_(Position(0, 0)) {};
    Move(const IPiece* const pPiece, const Position& from, const Position& to)
        : pPiece_(pPiece), from_(from), to_(to) {};
    virtual ~Move() = default;

    // Copy assignment operator
    Move& operator=(const Move& other) {
        // Self-assignment check
        if (this == &other)
            return *this;

        // Copy the data from the other object
        pPiece_ = other.pPiece_;
        from_ = other.from_;
        to_ = other.to_;

        return *this;
    }

    friend bool operator==(const Move& lhs, const Move& rhs) {
        return lhs.from_ == rhs.from_ && lhs.to_ == rhs.to_ && lhs.pPiece_ == rhs.pPiece_;
    }

    virtual const IPiece* getPiece() const { return pPiece_; }
    virtual const Position& getFrom() const { return from_; }
    virtual const Position& getTo() const { return to_; }

private:
    const IPiece* pPiece_;
    Position from_;
    Position to_;
};
