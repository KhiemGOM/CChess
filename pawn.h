//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H


#include "pieces.h"

class pawn : public pieces{
public:
    pawn(position _pos, color_enum _color) : pieces(_pos, _color, e_pawn) {};

    bool is_valid_move(board &game_board, position target) override;

    bool is_valid_capture(board &game_board, position target) override;

    bool is_obstruct(board &game_board, position target) override;
};


#endif //CHESS_PAWN_H