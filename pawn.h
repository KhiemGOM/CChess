//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H


#include "pieces.h"

class pawn : public pieces {
public:
    pawn(position _pos, color_enum _color) : pieces(_pos, _color, e_pawn) {};

    bool is_valid_move(const board& game_board, position target) const override;

    bool is_valid_capture(const board& game_board, position target) const override;

    bool is_obstruct(const board& game_board, position target) const override;

    void move_cleanup () override;

    pawn *clone() const override;
};


#endif //CHESS_PAWN_H