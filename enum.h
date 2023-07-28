//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_ENUM_H
#define CHESS_ENUM_H

#include <initializer_list>
#include <ranges>
#include <vector>
#include <algorithm>

enum color_enum
{
	e_black,
	e_white
};
enum type_enum
{
	e_empty, e_pawn, e_rook, e_knight, e_bishop, e_queen, e_king
};
enum _move_state
{
	invalid_move,
	move,
	capture,
	check,
	promotion,
	ks_castle,
	qs_castle
};

class move_state
{
public:
	std::vector<_move_state> states{};

	move_state(std::initializer_list<_move_state> list) : states(list)
	{};

	[[nodiscard]] bool contains(_move_state state) const
	{
		return std::ranges::any_of(states, [&](const auto& s)
		{ return s == state; });
	}

	move_state() = default;

	move_state(const move_state&) = default;

	move_state(move_state&&) noexcept = default;

	move_state& operator=(const move_state&) = default;

	move_state& operator=(move_state&&) noexcept = default;

	~move_state() = default;
};


#endif //CHESS_ENUM_H