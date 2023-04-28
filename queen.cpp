//
// Created by Admin on 25/03/2023.
//

#include "queen.h"
#include "rook.h"
#include "bishop.h"

bool queen::is_obstruct(const board& game_board, position target) const
{
    //Check if any pieces on the way to target excluding the target
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
    } else if (target.y == pos.y) {
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
    } else {
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
    }
    return false;
}

bool queen::is_valid_move(const board& game_board, position target) const
{
    //Queen can move both diagonally and straight
    return !is_obstruct(game_board, target) &&
           ((pos.x == target.x || pos.y == target.y || abs(pos.x - target.x) == abs(pos.y - target.y)));
}

bool queen::is_valid_capture(const board& game_board, position target) const
{
    return is_valid_move(game_board, target);
}

queen *queen::clone() const {
    return new queen(*this);
}