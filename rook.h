//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H


#include "pieces.h"

class rook : public pieces {
public:
    rook(position _pos, color_enum _color) : pieces(_pos, _color, e_rook) {};

    bool is_valid_move(board &game_board, position target) override;

    bool is_valid_capture(board &game_board, position target) override;

    bool is_obstruct(board &game_board, position target) override;
};


#endif //CHESS_ROOK_H