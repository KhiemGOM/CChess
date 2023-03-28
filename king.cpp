//
// Created by Admin on 25/03/2023.
//

#include "king.h"

bool king::is_obstruct(board &game_board, position target) {
    return false;
}

bool king::is_valid_move(board &game_board, position target) {
    return (abs(pos.x - target.x) <= 1 && abs(pos.y - target.y) <= 1);
}

bool king::is_valid_capture(board &game_board, position target) {
    return is_valid_move(game_board, target);
}

king *king::clone() const {
    return new king(*this);
}

void king::move_cleanup() {
    has_moved = true;
}