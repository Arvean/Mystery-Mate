#pragma once

#include <string>

class Position {
    public:
        Position() : file_(0), rank_(0) {};
        Position(char file, int rank)
            : file_(file), rank_(rank) {};
        virtual ~Position() = default;

        bool operator==(const Position& other) const {
            return this->file_ == other.file_ && this->rank_ == other.rank_;
        };

        const int getRank() const {return rank_;}
        const char getFile() const {return file_;}
    
    private:
        const int rank_;
        const char file_;
};


template<>
struct std::hash<Position> {
    size_t operator()(const Position& position) const {
        return std::hash<int>()(position.getRank()) ^ std::hash<char>()(position.getFile());
    }
};