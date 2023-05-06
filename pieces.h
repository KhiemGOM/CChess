//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "board.h"
#include "enum.h"
#include "move_result.h"

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

    bool operator==(const position &other) const = default;
};

struct standard_move
{
	position from {0, 0};
	position to {0, 0};
	move_state state {};
	type_enum promotion_type {};
	std::string err{};

	standard_move(position from, position to, move_state state, type_enum promotion_type = type_enum::e_empty)
			: from(from), to(to), state(state), promotion_type(promotion_type)
	{};
	standard_move(move_state state, std::string err): state(state), err(err){};
	standard_move() = default;
};
class pieces {
public:
    bool just_moved_2square{false};

    bool has_moved{false};

    static bool is_within_field(position pos);

    position pos{0, 0};
    color_enum color{e_white};
    type_enum type{e_pawn};

    [[nodiscard]] virtual bool is_valid_move(const board& game_board, position target) const = 0;

    [[nodiscard]] virtual bool is_valid_capture(const board& game_board, position target) const = 0;

    [[nodiscard]] virtual pieces *clone() const = 0;

    pieces(position _pos, color_enum _color, type_enum _type) : pos(_pos), color(_color), type(_type) {};

    pieces() = default;

    pieces(const pieces &a) = default;

	pieces(pieces&& a) = default;

	pieces& operator=(const pieces& other) = default;

	pieces& operator=(pieces&& other) = default;

    virtual ~pieces() = default;

    move_result move(board &game_board, position target, type_enum promotion_type);

    move_state try_to_move(board &game_board, position target, type_enum promotion_type) const;

    bool is_check(board &game_board, color_enum current_color) const;

    virtual void move_cleanup() = 0;

    [[nodiscard]] bool is_being_checked_after_move(const board &game_board, position target, color_enum current_color) const;

	[[nodiscard]] color_enum square_color() const;

protected:
    virtual bool is_obstruct(const board& game_board, position target) const = 0;
};


#endif //CHESS_PIECES_H