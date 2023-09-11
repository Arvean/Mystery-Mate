#pragma once

#include <string.h>
#include "piece.h"

class Player {
    public:
        Player() : horcruxe_id_(0), color_(Color::WHITE) {};
        Player(int horcruxe_id, const Color color)
            : horcruxe_id_{horcruxe_id}, color_{color} {};
        virtual ~Player() = default;

        int getHorcruxeID() const {return horcruxe_id_;}
        Color getColor() const {return color_;}

        Player& operator=(Player& other){
            if (this == &other) {return *this;}
            this->horcruxe_id_ = other.getHorcruxeID();
            this->color_ = other.getColor();
            return *this;
        }

    private:
        int horcruxe_id_;
        Color color_;
};