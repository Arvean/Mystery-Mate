#pragma once

#include <string>

template<>
struct std::hash<Position> {
    size_t operator()(const Position& position) const {
        return std::hash<int>()(position.getRank()) ^ std::hash<char>()(position.getFile());
    }
};

class Position {
    public:
        Position();
        Position(char file, int rank); // remove piece
        virtual ~Position() = default;

        const int getRank() const;
        const char getFile() const;
        //const std::string getPositionNotation() const;
    
    private:
        
        const int rank_;
        const char file_;
        //std::string positionNotation_;

}