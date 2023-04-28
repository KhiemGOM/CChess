//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H


#include "pieces.h"

class rook : public pieces {
public:

    rook(position _pos, color_enum _color) : pieces(_pos, _color, e_rook) {};

    bool is_valid_move(const board& game_board, position target) const override;

    bool is_valid_capture(const board& game_board, position target) const override;

    bool is_obstruct(const board& game_board, position target) const override;

    void move_cleanup() override;

    rook *clone() const override;
};


#endif //CHESS_ROOK_H