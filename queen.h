//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H


#include "pieces.h"

class queen : public pieces{
public:
    queen(position _pos, color_enum _color) : pieces(_pos, _color, e_queen) {};

    bool is_valid_move(board &game_board,position target) override;

    bool is_valid_capture(board &game_board,position target) override;

    bool is_obstruct(board &game_board,position target) override;

    void move_cleanup() override {};

    queen* clone() const override;
};


#endif //CHESS_QUEEN_H