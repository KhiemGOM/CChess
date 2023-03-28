//
// Created by Admin on 25/03/2023.
//

#include "knight.h"
#include <cmath>

bool knight::is_obstruct(board &game_board, position target) {
    return false;
}

bool knight::is_valid_move(board &game_board, position target) {
    //Knight can move only in L shape
    return std::pow(target.x - pos.x, 2) + std::pow(target.y - pos.y, 2) == 5;
}

bool knight::is_valid_capture(board &game_board, position target) {
    return is_valid_move(game_board, target);
}

knight *knight::clone() const {
    return new knight(*this);
}