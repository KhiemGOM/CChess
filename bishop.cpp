//
// Created by Admin on 25/03/2023.
//

#include "bishop.h"

bool bishop::is_obstruct(const board& game_board, position target) const
{
    int x = pos.x;
    int y = pos.y;
    while ((abs(x - target.x) > 1) && (abs(y - target.y) > 1)) {
        if (x < target.x) {
            x++;
        } else {
            x--;
        }
        if (y < target.y) {
            y++;
        } else {
            y--;
        }
        if (game_board.find({x, y}).has_value()) {
            return true;
        }
    }
    return false;
}

bool bishop::is_valid_move(const board& game_board, position target) const
{
    //Bishop can only move diagonally
    return !is_obstruct(game_board, target) && (abs(target.x - pos.x) == abs(target.y - pos.y));
}

bool bishop::is_valid_capture(const board& game_board, position target) const
{
    return is_valid_move(game_board, target);
}

bishop *bishop::clone() const {
    return new bishop(*this);
}