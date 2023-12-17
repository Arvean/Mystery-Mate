#pragma once

#include <string.h>
#include "piece.h"

#define INVALID_HORCRUXE_ID 0
#define MIN_WHITE_HORCRUXE_ID 1U
#define MIN_BLACK_HORCRUXE_ID 17U
#define MAX_HORCRUXE_ID 32U

#define NUMBER_OF_HORCRUX_GUESSES 2U

class Player {
    public:
        Player() 
            : horcrux_id_(INVALID_HORCRUXE_ID),
            color_(Color::WHITE),
            horcruxFound_(false), 
            horcruxCaptured_(false),
            hasKingBeenCaptured_(false),
            horcruxGuessLeft_(NUMBER_OF_HORCRUX_GUESSES){}

        explicit Player(Color color) 
            : horcrux_id_(INVALID_HORCRUXE_ID),
            color_(color),
            horcruxFound_(false),
            horcruxCaptured_(false),
            hasKingBeenCaptured_(false),
            horcruxGuessLeft_(NUMBER_OF_HORCRUX_GUESSES) {}
        
        virtual ~Player() = default;

        virtual int getHorcruxID() const { 
            return horcrux_id_; 
        }

        virtual bool getHorcruxFound() const {
            return horcruxFound_;
        }

        virtual void setHasKingBeenCaptured() {
            hasKingBeenCaptured_ = true;
        }

        virtual bool getHasKingBeenCaptured() const {
            return hasKingBeenCaptured_;
        }

        virtual void setHasHorcruxBeenCaptured() {
            horcruxCaptured_ = true;
        }

        virtual bool hasHorcruxBeenCaptured() const {
            return horcruxCaptured_;
        }

        virtual int getNumberOfHorcruxGuessesLeft() const {
            return horcruxGuessLeft_;
        }

        virtual void decrementNumberOfHorcruxGuessesleft() {
            horcruxGuessLeft_--;
        }

        virtual Color getColor() const { return color_; }

        virtual void setHorcruxID(int horcruxID) {
            if (_validateHorcruxID(horcruxID)) {
                horcrux_id_ = horcruxID;
            };
        }

        virtual void setHorcruxFound() { horcruxFound_ = true; }

        Player& operator=(const Player& other) {
            if (this != &other) {
                this->horcrux_id_ = other.getHorcruxID();
                this->color_ = other.getColor();
                this->horcruxFound_ = other.getHorcruxFound();
            }
            return *this;
        }

    private:
        bool _validateHorcruxID(int horcruxID) const {
            if (color_ == Color::WHITE && (horcruxID < MIN_WHITE_HORCRUXE_ID || horcruxID >= MIN_BLACK_HORCRUXE_ID)) {
                throw std::logic_error("White player must have horcrux ID starting at the white range");
                return false;
            } else {
                return true;
            }
            if (color_ == Color::BLACK && (horcruxID < MIN_BLACK_HORCRUXE_ID || horcruxID > MAX_HORCRUXE_ID)) {
                throw std::logic_error("Black player must have horcrux ID in the black range");
                return false;
            } else {
                return true;
            }
        }

        bool horcruxFound_;
        int horcrux_id_;
        Color color_;
        bool hasKingBeenCaptured_;
        bool horcruxCaptured_;
        int horcruxGuessLeft_;
    };
