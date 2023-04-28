//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H


#include "pieces.h"

class knight : public pieces {
public:
    knight(position _pos, color_enum _color) : pieces(_pos, _color, e_knight) {};

    bool is_valid_move(const board& game_board, position target) const override;

    bool is_valid_capture(const board& game_board, position target) const override;

    bool is_obstruct(const board& game_board, position target) const override;

    knight* clone() const override;

    void move_cleanup () override {};
};


#endif //CHESS_KNIGHT_H