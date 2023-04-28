//
// Created by Admin on 25/03/2023.
//

#include "pawn.h"

bool pawn::is_obstruct(const board& game_board, position target) const
{
    //Check if obstructed in the move-2-square-on-first-move case
    if (target.y == pos.y + 2 && pos.y == 1 && color == e_white) {
        return game_board.find({pos.x, pos.y + 1}).has_value();
    }
    if (target.y == pos.y - 2 && pos.y == 6 && color == e_black) {
        return game_board.find({pos.x, pos.y - 1}).has_value();
    }
    return false;
}

bool pawn::is_valid_move(const board& game_board, position target) const
{
    //Pawn can move only straight 1 or 2 on the first move
    return !is_obstruct(game_board, target)
           && (target.x == pos.x && target.y == pos.y + 1 && color == e_white)
           || (target.x == pos.x && target.y == pos.y - 1 && color == e_black)
           || (target.x == pos.x && target.y == pos.y + 2 && pos.y == 1 && color == e_white)
           || (target.x == pos.x && target.y == pos.y - 2 && pos.y == 6 && color == e_black);
}

bool pawn::is_valid_capture(const board& game_board, position target) const
{
    //Pawn can capture only diagonally
    return (target.x == pos.x + 1 && target.y == pos.y + 1 && color == e_white)
           || (target.x == pos.x - 1 && target.y == pos.y + 1 && color == e_white)
           || (target.x == pos.x + 1 && target.y == pos.y - 1 && color == e_black)
           || (target.x == pos.x - 1 && target.y == pos.y - 1 && color == e_black);
}

pawn *pawn::clone() const {
    return new pawn(*this);
}

void pawn::move_cleanup() {
    just_moved_2square = true;
}