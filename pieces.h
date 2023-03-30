//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "board.h"
#include "enum.h"

class board;

/**
 * @brief Class for storing position of a piece.
 * x is horizontal position, y is vertical position.
 * x = 0 is the leftmost column, y = 0 is the bottom row
 */
class position {
public:
    int x{0}, y{0};

    position(int _x, int _y) : x(_x), y(_y) {};

    position() = default;

    bool operator==(const position &other) const {
        return x == other.x && y == other.y;
    }
};


class pieces {
public:
    bool just_moved_2square{false};

    bool has_moved{false};

    static bool is_within_field(position pos);

    position pos{0, 0};
    color_enum color{e_white};
    type_enum type{e_pawn};

    virtual bool is_valid_move(board &game_board, position target) = 0;

    virtual bool is_valid_capture(board &game_board, position target) = 0;

    virtual pieces *clone() const = 0;

    pieces(position _pos, color_enum _color, type_enum _type) : pos(_pos), color(_color), type(_type) {};

    pieces() = default;

    pieces(const pieces &a) = default;

    virtual ~pieces() = default;

    enum move_state move(board &game_board, position target);

    move_state try_to_move(board &game_board, position target);

    bool is_check(board &game_board, color_enum _color);

    virtual void move_cleanup() = 0;

    [[nodiscard]] bool is_being_checked(const board &game_board_copy, position target, color_enum _color) const;

protected:
    virtual bool is_obstruct(board &game_board, position target) = 0;
};


#endif //CHESS_PIECES_H