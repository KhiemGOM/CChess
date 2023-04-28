//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H


#include "pieces.h"

class bishop : public pieces {
public:
    bishop(position _pos, color_enum _color) : pieces(_pos, _color, e_bishop) {};

    bool is_valid_move(const board& game_board, position target) const override;

    bool is_valid_capture(const board& game_board, position target) const override;

    bool is_obstruct(const board& game_board, position target) const override;

    bishop* clone() const override;

    void move_cleanup () override {};
};


#endif //CHESS_BISHOP_H