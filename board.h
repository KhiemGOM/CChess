//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <string>
#include <vector>
#include <array>
#include <memory>
#include "pieces.h"
#include <optional>
#include "enum.h"

class position;
class standard_move;

class pieces;


class board
{
	static const int NUM_OF_PIECES = 32;
public:
	std::array<std::shared_ptr<pieces>, NUM_OF_PIECES> val;

	board(const board& b) = default;

	board(board&& b) = default;

	board() = default;

	~board() = default;

	board& operator=(const board& other) = default;

	board& operator=(board&& other) = default;

	board(std::initializer_list<std::shared_ptr<pieces>> list);

	[[nodiscard]] std::optional<std::reference_wrapper<const std::shared_ptr<pieces>>> find(position pos) const;

	[[nodiscard]] std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> find(position pos);

	[[nodiscard]] std::vector<std::reference_wrapper<const std::shared_ptr<pieces>>> find_all(type_enum type, enum color_enum color, auto condition) const;

	[[nodiscard]] std::optional<std::reference_wrapper<const std::shared_ptr<pieces>>>
	find(type_enum type, enum color_enum color) const;

	[[nodiscard]] std::optional<std::reference_wrapper<std::shared_ptr<pieces>>>
	find(type_enum type, enum color_enum color);

	[[nodiscard]] bool is_out_of_moves(color_enum current_color) const;

	[[nodiscard]] board& clone() const;

	[[nodiscard]] bool able_to_castle(const std::shared_ptr<pieces>& king, const std::shared_ptr<pieces>& rook) const;

	[[nodiscard]] bool is_being_checked(color_enum current_color);

	bool promote(std::shared_ptr<pieces>& piece, position target, color_enum new_color);

	void promote(std::shared_ptr<pieces>& piece, position target, color_enum new_color, type_enum new_type);

	[[nodiscard]] short compare(const board& other) const;

	[[nodiscard]] standard_move interpret(std::string input, color_enum color) const;

	[[nodiscard]] bool is_insufficient_material () const;

	[[nodiscard]] std::string save(color_enum turn, int half_move, int full_move) const;

	std::string load(std::string_view input, int& half_move, int& full_move, color_enum& turn);
};

#endif //CHESS_BOARD_H