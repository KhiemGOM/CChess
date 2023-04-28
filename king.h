//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_KING_H
#define CHESS_KING_H


#include "pieces.h"

class king : public pieces {
public:

    king(const position& _pos, color_enum _color) : pieces(_pos, _color, e_king) {};

    bool is_valid_move(const board& game_board, position target) const override;

    bool is_valid_capture(const board& game_board, position target) const override;

    bool is_obstruct(const board& game_board, position target) const override;

    void move_cleanup () override;

    king* clone() const override;

    ~king() override = default;
};


#endif //CHESS_KING_H