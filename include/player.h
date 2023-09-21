#pragma once

#include <string.h>
#include "piece.h"

#define MIN_WHITE_HORCRUXE_ID 1U
#define MIN_BLACK_HORCRUXE_ID 17U
#define MAX_HORCRUXE_ID 32U


class Player {
public:

    Player() : horcruxe_id_(MIN_WHITE_HORCRUXE_ID), 
               color_(Color::WHITE) {};
    
    Player(int horcruxe_id, const Color color)
        : horcruxe_id_{horcruxe_id}, color_{color} {
        validateHorcruxeID();
    };
    
    virtual ~Player() = default;

    virtual int getPlayerID() const {return player_id_;}
    virtual int getHorcruxeID() const { return horcruxe_id_; }
    virtual Color getColor() const { return color_; }

    virtual Player& operator=(Player& other) {
        if (this == &other) { return *this; }
        this->horcruxe_id_ = other.getHorcruxeID();
        this->color_ = other.getColor();
        validateHorcruxeID();  // Ensure the ID is valid after assignment
        return *this;
    }

private:
    void validateHorcruxeID() {
        if (color_ == Color::WHITE && (horcruxe_id_ < MIN_WHITE_HORCRUXE_ID || horcruxe_id_ >= MIN_BLACK_HORCRUXE_ID)) {
            throw std::logic_error("White player must have horcruxe ID starting at 1");
        } 
        if (color_ == Color::BLACK && (horcruxe_id_ < MIN_BLACK_HORCRUXE_ID || horcruxe_id_ > MAX_HORCRUXE_ID)) {
            throw std::logic_error("Black player must have horcruxe ID starting at 17");
        }
    }

    int player_id_;
    int horcruxe_id_;
    Color color_;
};
