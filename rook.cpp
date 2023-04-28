//
// Created by Admin on 25/03/2023.
//

#include "rook.h"

bool rook::is_obstruct(const board& game_board, position target) const
{
    if (target.x == pos.x) {
        int y = pos.y;
        while (abs(y - target.y) > 1) {
            if (y < target.y) {
                y++;
            } else {
                y--;
            }
            if (game_board.find({pos.x, y}).has_value()) {
                return true;
            }
        }
    } else {
        int x = pos.x;
        while (abs(x - target.x) > 1) {
            if (x < target.x) {
                x++;
            } else {
                x--;
            }
            if (game_board.find({x, pos.y}).has_value()) {
                return true;
            }
        }
    }
    return false;
}

bool rook::is_valid_move(const board& game_board, position target) const
{
    //Rook can move only straight
    return !is_obstruct(game_board, target) && (target.x == pos.x || target.y == pos.y);
}

bool rook::is_valid_capture(const board& game_board, position target) const
{
    return is_valid_move(game_board, target);
}

rook *rook::clone() const {
    return new rook(*this);
}

void rook::move_cleanup() {
    has_moved = true;
}