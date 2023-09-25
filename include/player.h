#pragma once

#include <string.h>
#include "piece.h"

#define INVALID_HORCRUXE_ID 0 // Need to remove when refactoring
#define MIN_WHITE_HORCRUXE_ID 1U
#define MIN_BLACK_HORCRUXE_ID 17U
#define MAX_HORCRUXE_ID 32U


class Player {
public:

    Player() : horcruxe_id_(MIN_WHITE_HORCRUXE_ID), 
               color_(Color::WHITE) {};
    
    Player(Color color)
        : color_{color} {
        _validateHorcruxeID();
    }
    
    virtual ~Player() = default;

    virtual int getHorcruxeID() const { return horcruxe_id_; }
    virtual Color getColor() const { return color_; }
    virtual void setHorcruxeID(int horcruxeID) {horcruxe_id_ = horcruxeID; _validateHorcruxeID();}

    virtual Player& operator=(Player& other) {
        if (this == &other) { return *this; }
        this->horcruxe_id_ = other.getHorcruxeID();
        this->color_ = other.getColor();
        _validateHorcruxeID();  // Ensure the ID is valid after assignment
        return *this;
    }

private:
    void _validateHorcruxeID() {
        if (color_ == Color::WHITE && (horcruxe_id_ < MIN_WHITE_HORCRUXE_ID || horcruxe_id_ >= MIN_BLACK_HORCRUXE_ID)) {
            throw std::logic_error("White player must have horcruxe ID starting at 1");
        } 
        if (color_ == Color::BLACK && (horcruxe_id_ < MIN_BLACK_HORCRUXE_ID || horcruxe_id_ > MAX_HORCRUXE_ID)) {
            throw std::logic_error("Black player must have horcruxe ID starting at 17");
        }
    }

    int horcruxe_id_ = INVALID_HORCRUXE_ID;
    Color color_;
};
