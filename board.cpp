//
// Created by Admin on 25/03/2023.
//

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <algorithm>
#include <ranges>
#include "board.h"
#include "enum.h"
#include "rook.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"

std::optional<std::reference_wrapper<const std::shared_ptr<pieces>>> board::find(position pos) const
{
	for (const std::shared_ptr<pieces>& piece: val)
	{
		if (piece != nullptr)
		{
			if (piece->pos.x == pos.x && piece->pos.y == pos.y)
			{
				return {piece};
			}
		}
	}
	return {};
}

std::optional<std::reference_wrapper<const std::shared_ptr<pieces>>>
board::find(type_enum type, enum color_enum color) const
{
	for (const std::shared_ptr<pieces>& piece: val)
	{
		if (piece != nullptr)
		{
			if (piece->type == type && piece->color == color)
			{
				return {piece};
			}
		}
	}
	return {};
}

bool board::is_out_of_moves(color_enum _color) const
{
	//Check if all pieces are out of moves
	auto game_board_copy = clone();
	for (auto& piece: game_board_copy.val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->color == _color)
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						for (type_enum type: {type_enum::e_king, type_enum::e_queen,
											  type_enum::e_rook, type_enum::e_bishop, type_enum::e_knight,
											  type_enum::e_pawn})
						{
							if (piece->try_to_move(game_board_copy, {i, j}, type) != move_state::invalid_move)
							{
								return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

board::board(std::initializer_list<std::shared_ptr<pieces>> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		val[i] = *(list.begin() + i);
	}
}

board& board::clone() const
{
	auto* b = new board();
	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		if (val[i].get() != nullptr)
		{
			auto *temp = val[i]->clone();
			b->val[i].reset(temp);
		}
	}
	return *b;
}

bool board::able_to_castle(const std::shared_ptr<pieces>& king_o, const std::shared_ptr<pieces>& rook_o) const
{
	auto copy_board = clone();
	std::weak_ptr<pieces> king_ptr = std::weak_ptr<pieces> {copy_board.find(king_o->pos).value().get()};
	std::weak_ptr<pieces> rook_ptr = std::weak_ptr<pieces> {copy_board.find(rook_o->pos).value().get()};
	//Check if king in check
	for (auto& piece: copy_board.val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->color != king_ptr.lock()->color)
			{
				if (piece->is_valid_capture(copy_board, king_ptr.lock()->pos))
				{
					return false;
				}
			}
		}
	}
	//Check between the king_ptr and the rook_ptr if the space is occupied or is under controlled by another pieces of opposite color
	if (king_ptr.lock()->pos.x < rook_ptr.lock()->pos.x)
	{
		for (int i = king_ptr.lock()->pos.x + 1; i < rook_ptr.lock()->pos.x; i++)
		{
			if (copy_board.find({i, king_ptr.lock()->pos.y}).has_value())
			{
				return false;
			}
			for (auto& piece: copy_board.val)
			{
				if (piece.get() != nullptr)
				{
					if (piece->color != king_ptr.lock()->color)
					{
						if (piece->is_valid_capture(copy_board, {i, king_ptr.lock()->pos.y}))
						{
							return false;
						}
					}
				}
			}
		}
	}
	else
	{
		for (int i = rook_ptr.lock()->pos.x + 1; i < king_ptr.lock()->pos.x; i++)
		{
			if (copy_board.find({i, king_ptr.lock()->pos.y}).has_value())
			{
				return false;
			}
			for (auto& piece: copy_board.val)
			{
				if (piece.get() != nullptr)
				{
					if (piece->color != king_ptr.lock()->color)
					{
						if (piece->is_valid_capture(copy_board, {i, king_ptr.lock()->pos.y}))
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

//Color is the color of the side wanting to be checked
bool board::is_being_checked(color_enum _color)
{
	for (auto& piece: val)
	{
		if (piece.get() != nullptr && piece->color != _color && piece->is_check(*this, _color))
		{
			return true;
		}
	}
	return false;
}

//Return true if the input was successful
bool board::promote(std::shared_ptr<pieces>& piece, position target, color_enum new_color)
{
	std::cout << "Enter the piece you want to promote to (q, r, b, n): ";
	std::string input2;
	getline(std::cin, input2);
	std::shared_ptr<pieces>& piece_to_promote = find(piece->pos).value().get(); //NOLINT
	if (input2 == "q")
	{
		piece_to_promote = std::make_shared<queen>(target, new_color);
		return true;
	}
	if (input2 == "r")
	{
		piece_to_promote = std::make_shared<rook>(target, new_color);
		return true;
	}
	if (input2 == "b")
	{
		piece_to_promote = std::make_shared<bishop>(target, new_color);
		return true;
	}
	if (input2 == "n")
	{
		piece_to_promote = std::make_shared<knight>(target, new_color);
		return true;
	}
	return false;
}

void board::promote(std::shared_ptr<pieces>& piece, position target, color_enum new_color, type_enum new_type)
{
	switch (new_type)
	{
	case type_enum::e_queen:
		piece.reset(new queen {target, new_color});
		break;
	case type_enum::e_rook:
		piece.reset(new rook {target, new_color});
		break;
	case type_enum::e_bishop:
		piece.reset(new bishop {target, new_color});
		break;
	case type_enum::e_knight:
		piece.reset(new knight {target, new_color});
		break;
	case type_enum::e_empty:
		promote(piece, target, new_color);
	default:
		break;
	}
}

//Compare the two board, return 0 if they are the same (including castling right and en passant right)
//Return 1 if the board is not the same but have the same pieces but on different squares
//Return -1 if the board doesn't have the same piece or castling right and en passant right is different
short board::compare(const board& other) const
{
	bool diff_pos = false;
	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		if (val[i].get() != nullptr && other.val[i].get() != nullptr)
		{
			if (val[i]->pos != other.val[i]->pos)
			{
				diff_pos = true;
				const auto result = other.find(val[i]->pos);
				if (!result.has_value())
				{
					return -1;
				}
				const auto temp = result.value().get();
				if (val[i]->type != temp->type)
				{
					return -1;
				}
				if (val[i]->color != temp->color)
				{
					return -1;
				}
				//Check for en passant and castling
				if ((temp->type == e_pawn || temp->type == e_king || temp->type == e_rook) &&
					(temp->has_moved != other.val[i]->has_moved))
				{
					return -1;
				}
			}
		}
		else
		{
			//No piece in *this[i]
			if (other.val[i].get() != nullptr)
			{
				//There is a piece in other[i]
				const auto result = find(other.val[i]->pos);
				if (result.has_value())
				{
					return -1;
				}
			}
		}
	}
	return diff_pos ? 1 : 0;
}

std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> board::find(type_enum type, enum color_enum color)
{
	for (auto& piece: val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->type == type && piece->color == color)
			{
				return {piece};
			}
		}
	}
	return {};
}

std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> board::find(position pos)
{
	for (auto& piece: val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->pos == pos)
			{
				return {piece};
			}
		}
	}
	return {};
}

bool is_file(char k)
{
	return k >= 'a' && k <= 'h';
}

bool is_rank(char k)
{
	return k >= '1' && k <= '8';
}

type_enum to_type(char k)
{
	switch (k)
	{
	case 'N':
		return e_knight;
	case 'B':
		return e_bishop;
	case 'R':
		return e_rook;
	case 'Q':
		return e_queen;
	case 'K':
		return e_king;
	default:
		return e_empty;
	}
}

position to_position(const std::string& input)
{
	if (input.size() != 2)
	{
		return {-1, -1};
	}
	if (!is_file(input[0]) || !is_rank(input[1]))
	{
		return {-1, -1};
	}
	return {input[0] - 'a', input[1] - '1'};
}

std::string type_to_string(type_enum type)
{
	switch (type)
	{
	case e_king:
		return "king";
	case e_queen:
		return "queen";
	case e_rook:
		return "rook";
	case e_knight:
		return "knight";
	case e_bishop:
		return "bishop";
	case e_pawn:
		return "pawn";
	default:
		return "unknown piece";
	}
}

standard_move board::interpret(std::string input, color_enum color) const
{
	auto [ret, last] = std::ranges::remove_if(input, [](const char& k)
	{
		return k == '+' || k == '#';
	});
	input = std::string(input.begin(), ret);
	if (input == "O-O")
	{
		auto king = find(e_king, color)->get();
		auto rook = find({7, king->pos.y});
		if (!rook.has_value())
		{
			return {move_state::invalid_move, "Cannot castle"};
		}
		return {king->pos, rook->get()->pos, move};
	}
	if (input == "O-O-O")
	{
		auto king = find(e_king, color)->get();
		auto rook = find({0, king->pos.y});
		if (!rook.has_value())
		{
			return {move_state::invalid_move, "Cannot castle"};
		}
		return {king->pos, rook->get()->pos, move};
	}
	auto only_pos = to_position(input);
	if (only_pos.x != -1)
	{
		//Only a position is given (pawn move promotion) (e4=Q)
		type_enum promotion = e_empty;
		if (input.size() == 4 && input[2] == '=')
		{
			promotion = to_type(input.back());
			if (promotion == e_empty)
			{
				return {move_state::invalid_move, "Invalid promotion"};
			}
		}
		//Only a position is given (pawn move) (e4)
		if (color == e_black)
		{
			for (int ind = only_pos.y + 1; ind < 8; ind++)
			{
				auto result = find({only_pos.x, ind});
				if (!result.has_value())
				{
					continue;
				}
				if (result.value().get()->type != e_pawn)
				{
					continue;
				}
				return {result.value().get()->pos, only_pos, move_state::move, promotion};
			}
			return {invalid_move, "No pawn on the " + std::string(1, input[0]) + " file that can move there"};
		}

		for (int ind = only_pos.y - 1; ind >= 0; ind--)
		{
			auto result = find({only_pos.x, ind});
			if (!result.has_value())
			{
				continue;
			}
			if (result.value().get()->type != e_pawn)
			{
				continue;
			}
			return {result.value().get()->pos, only_pos, move_state::move};
		}
		return {invalid_move, "No pawn on the " + std::string(1, input[0]) + " file that can move there"};
	}

	bool is_capture = std::ranges::any_of(input, [](char k)
	{ return k == 'x'; });
	if (is_capture)
	{
		if (is_file(input[0]))
		{
			//Capture with pawn and promote (exd8=Q)
			type_enum promotion = e_empty;
			if (input.size() == 6 && input[4] == '=')
			{
				promotion = to_type(input.back());
				if (promotion == e_empty)
				{
					return {move_state::invalid_move, "Invalid promotion"};
				}
			}
			//Capture with pawn (exd4)
			position to = to_position(input.substr(2, 2));
			if (to.x == -1)
			{
				return {move_state::invalid_move, "Invalid position"};
			}
			if (color == e_black)
			{
				for (int ind = to.y + 1; ind < 8; ind++)
				{
					auto result = find({input[0] - 'a', ind});
					if (!result.has_value())
					{
						continue;
					}
					if (result.value().get()->type != e_pawn)
					{
						continue;
					}
					return {result.value().get()->pos, to, move_state::capture, promotion};
				}
				return {invalid_move, "No pawn on the " + std::string(1, input[0]) + " file that can capture there"};
			}
			for (int ind = to.y - 1; ind >= 0; ind--)
			{
				auto result = find({input[0] - 'a', ind});
				if (!result.has_value())
				{
					continue;
				}
				if (result.value().get()->type != e_pawn)
				{
					continue;
				}
				return {result.value().get()->pos, to, move_state::capture, promotion};
			}
			return {invalid_move, "No pawn on the " + std::string(1, input[0]) + " file that can capture there"};
		}
		//Capture with other piece
		auto type = to_type(input[0]);
		if (type == e_empty)
		{
			return {invalid_move, "Invalid piece notation"};
		}
		if (is_file(input[1]))
		{
			//File disambiguation (Ndxe5)
			auto to = to_position(input.substr(3, 2));
			if (to.x == -1)
			{
				return {move_state::invalid_move, "Invalid position"};
			}
			auto file = input[1] - 'a';
			auto candidates = find_all(type, color, [this, &file, &to](const auto& a)
			{
				return a->pos.x == file && a->is_valid_capture(*this, to);
			});
			if (candidates.empty())
			{
				return {invalid_move, "No " + type_to_string(type) + " on " + std::string(1, input[1]) +
									  " file that can capture there"};
			}
			if (candidates.size() > 1)
			{
				return {invalid_move, "Please provide rank disambiguation"};
			}
			return {candidates[0].get()->pos, to, capture};
		}
		if (is_rank(input[1]))
		{
			//Rank disambiguation (N1xe5)
			auto to = to_position(input.substr(3, 2));
			if (to.x == -1)
			{
				return {move_state::invalid_move, "Invalid position"};
			}
			auto rank = input[1] - '1';
			auto candidates = find_all(type, color, [this, &rank, &to](const auto& a)
			{
				return a->pos.y == rank && a->is_valid_capture(*this, to);
			});
			if (candidates.empty())
			{
				return {invalid_move,
						"No " + type_to_string(type) + " on " + std::to_string(rank) + " rank that can capture there"};
			}
			if (candidates.size() > 1)
			{
				return {invalid_move, "Please provide file disambiguation"};
			}
			return {candidates[0].get()->pos, to, capture};
		}
		if (input[1] == 'x')
		{
			//No disambiguation (Nxe5)
			auto to = to_position(input.substr(2, 2));
			if (to.x == -1)
			{
				return {move_state::invalid_move, "Invalid position"};
			}
			auto candidates = find_all(type, color, [this, &to](const auto& a)
			{
				return a->is_valid_capture(*this, to);
			});
			if (candidates.empty())
			{
				return {invalid_move, "No " + type_to_string(type) + " on board that can capture there"};
			}
			if (candidates.size() > 1)
			{
				return {invalid_move, "Please provide disambiguation"};
			}
			return {candidates[0].get()->pos, to, capture};
		}
		return {invalid_move, "Unknown notation"};
	}
	auto type = to_type(input[0]);
	if (type == e_empty)
	{
		return {invalid_move, "Invalid piece notation"};
	}
	if (is_file(input[1]) && input.size() == 4)
	{
		//File disambiguation (Nde5)
		auto to = to_position(input.substr(2, 2));
		if (to.x == -1)
		{
			return {move_state::invalid_move, "Invalid position"};
		}
		auto file = input[1] - 'a';
		auto candidates = find_all(type, color, [this, &file, &to](const auto& a)
		{
			return a->pos.x == file && a->is_valid_move(*this, to);
		});
		if (candidates.empty())
		{
			return {invalid_move,
					"No " + type_to_string(type) + " on " + std::string(1, input[1]) + " file that can move there"};
		}
		if (candidates.size() > 1)
		{
			return {invalid_move, "Please provide rank disambiguation"};
		}
		return {candidates[0].get()->pos, to, move};
	}
	if (is_rank(input[1]) && input.size() == 4)
	{
		//Rank disambiguation (N1e5)
		auto to = to_position(input.substr(2, 2));
		if (to.x == -1)
		{
			return {move_state::invalid_move, "Invalid position"};
		}
		auto rank = input[1] - '1';
		auto candidates = find_all(type, color, [this, &rank, &to](const auto& a)
		{
			return a->pos.y == rank && a->is_valid_move(*this, to);
		});
		if (candidates.empty())
		{
			return {invalid_move,
					"No " + type_to_string(type) + " on " + std::to_string(rank) + " rank that can move there"};
		}
		if (candidates.size() > 1)
		{
			return {invalid_move, "Please provide file disambiguation"};
		}
		return {candidates[0].get()->pos, to, move};
	}

	//No disambiguation (Ne5)
	auto to = to_position(input.substr(1, 2));
	if (to.x == -1)
	{
		return {move_state::invalid_move, "Invalid position"};
	}
	auto candidates = find_all(type, color, [this, &to](const auto& a)
	{
		return a->is_valid_move(*this, to);
	});
	if (candidates.empty())
	{
		return {invalid_move, "No " + type_to_string(type) + " on board that can move there"};
	}
	if (candidates.size() > 1)
	{
		return {invalid_move, "Please provide disambiguation"};
	}
	return {candidates[0].get()->pos, to, move};
}

std::vector<std::reference_wrapper<const std::shared_ptr<pieces>>>
board::find_all(type_enum type, enum color_enum color, auto condi) const
{
	std::vector<std::reference_wrapper<const std::shared_ptr<pieces>>> result {};
	for (const auto& piece: val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->type == type && piece->color == color && condi(piece))
			{
				result.emplace_back(piece);
			}
		}
	}
	return result;
}

bool board::is_insufficient_material() const
{
	std::map<type_enum, int> white_pieces {};
	std::map<type_enum, int> black_pieces {};
	for (const auto& piece: val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->color == e_white)
			{
				white_pieces[piece->type]++;
			}
			else
			{
				black_pieces[piece->type]++;
			}
		}
	}
	/*
	 * king versus king
	 * king and bishop versus king
	 * king and knight versus king
	 * king and bishop versus king and bishop with the bishops on the same color.
	 * (Don't check for king as they always = 1)
	 * */
	if (white_pieces.size() == 1 && black_pieces.size() == 1)
	{
		return true;
	}
	if (white_pieces.size() == 2 && black_pieces.size() == 1)
	{
		if (white_pieces[e_bishop] == 1 || white_pieces[e_knight] == 1)
		{
			return true;
		}
	}
	if (white_pieces.size() == 1 && black_pieces.size() == 2)
	{
		if (black_pieces[e_bishop] == 1 || black_pieces[e_knight] == 1)
		{
			return true;
		}
	}
	if (white_pieces.size() == 2 && black_pieces.size() == 2)
	{
		if (white_pieces[e_bishop] == 1 && black_pieces[e_bishop] == 1)
		{
			auto white_bishop = find_all(e_bishop, e_white, [](const auto& a)
			{
				return a->square_color() == e_white;
			});
			auto black_bishop = find_all(e_bishop, e_black, [](const auto& a)
			{
				return a->square_color() == e_black;
			});
			if (white_bishop.size() == 1 && black_bishop.size() == 1)
			{
				return true;
			}
			white_bishop = find_all(e_bishop, e_black, [](const auto& a)
			{
				return a->square_color() == e_white;
			});
			black_bishop = find_all(e_bishop, e_white, [](const auto& a)
			{
				return a->square_color() == e_black;
			});
			if (white_bishop.size() == 1 && black_bishop.size() == 1)
			{
				return true;
			}
		}
	}
	return false;
}