#include "position.h"


Position::Position(char rank, int file)
    : rank_(rank), file_(file) {};


const char Position::getRank() const {return rank_};

const int Position::getFile() const {return file_};

/*
const std::string Position::getPositionNotation const {
    
};
*/