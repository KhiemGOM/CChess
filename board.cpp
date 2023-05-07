//
// Created by Admin on 25/03/2023.
//

#include <ranges>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <algorithm>
#include <sstream>
#include "board.h"
#include "enum.h"
#include "rook.h"
#include "queen.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
#include "king.h"

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

bool board::is_out_of_moves(color_enum current_color) const
{
	//Check if all pieces are out of moves
	auto game_board_copy = clone();
	for (auto& piece: game_board_copy.val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->color == current_color)
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
			auto* temp = val[i]->clone();
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
bool board::is_being_checked(color_enum current_color)
{
	return std::ranges::any_of(val, [&current_color, this](const std::shared_ptr<pieces>& piece)
	{
		return piece != nullptr && piece->color != current_color && piece->is_check(*this, current_color);
	});
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
		case type_enum::e_queen: piece.reset(new queen {target, new_color});
			break;
		case type_enum::e_rook: piece.reset(new rook {target, new_color});
			break;
		case type_enum::e_bishop: piece.reset(new bishop {target, new_color});
			break;
		case type_enum::e_knight: piece.reset(new knight {target, new_color});
			break;
		case type_enum::e_empty: promote(piece, target, new_color);
		default: break;
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

bool IsFile(char k)
{
	return k >= 'a' && k <= 'h';
}

bool IsRank(char k)
{
	return k >= '1' && k <= '8';
}

type_enum ToType(char k)
{
	switch (k)
	{
		case 'N': return e_knight;
		case 'B': return e_bishop;
		case 'R': return e_rook;
		case 'Q': return e_queen;
		case 'K': return e_king;
		case 'P': return e_pawn;
		default: return e_empty;
	}
}

position ToPosition(const std::string& input)
{
	if (input.size() != 2)
	{
		return {-1, -1};
	}
	if (!IsFile(input[0]) || !IsRank(input[1]))
	{
		return {-1, -1};
	}
	return {input[0] - 'a', input[1] - '1'};
}

std::string TypeToString(type_enum type)
{
	switch (type)
	{
		case e_king: return "king";
		case e_queen: return "queen";
		case e_rook: return "rook";
		case e_knight: return "knight";
		case e_bishop: return "bishop";
		case e_pawn: return "pawn";
		default: return "unknown piece";
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
		auto king = find(e_king, color)->get(); // NOLINT(bugprone-unchecked-optional-access)
		auto rook = find({7, king->pos.y});
		if (!rook.has_value())
		{
			return {move_state::invalid_move, "Cannot castle"};
		}
		return {king->pos, rook->get()->pos, move};
	}
	if (input == "O-O-O")
	{
		auto king = find(e_king, color)->get(); // NOLINT(bugprone-unchecked-optional-access)
		auto rook = find({0, king->pos.y});
		if (!rook.has_value())
		{
			return {move_state::invalid_move, "Cannot castle"};
		}
		return {king->pos, rook->get()->pos, move};
	}
	auto only_pos = ToPosition(input);
	if (only_pos.x != -1)
	{
		//Only a position is given (pawn move promotion) (e4=Q)
		type_enum promotion = e_empty;
		if (input.size() == 4 && input[2] == '=')
		{
			promotion = ToType(input.back());
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
		if (IsFile(input[0]))
		{
			//Capture with pawn and promote (exd8=Q)
			type_enum promotion = e_empty;
			if (input.size() == 6 && input[4] == '=')
			{
				promotion = ToType(input.back());
				if (promotion == e_empty)
				{
					return {move_state::invalid_move, "Invalid promotion"};
				}
			}
			//Capture with pawn (exd4)
			position to = ToPosition(input.substr(2, 2));
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
		auto type = ToType(input[0]);
		if (type == e_empty)
		{
			return {invalid_move, "Invalid piece notation"};
		}
		if (IsFile(input[1]))
		{
			//File disambiguation (Ndxe5)
			auto to = ToPosition(input.substr(3, 2));
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
				return {invalid_move, "No " + TypeToString(type) + " on " + std::string(1, input[1]) +
									  " file that can capture there"};
			}
			if (candidates.size() > 1)
			{
				return {invalid_move, "Please provide rank disambiguation"};
			}
			return {candidates[0].get()->pos, to, capture};
		}
		if (IsRank(input[1]))
		{
			//Rank disambiguation (N1xe5)
			auto to = ToPosition(input.substr(3, 2));
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
						"No " + TypeToString(type) + " on " + std::to_string(rank) + " rank that can capture there"};
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
			auto to = ToPosition(input.substr(2, 2));
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
				return {invalid_move, "No " + TypeToString(type) + " on board that can capture there"};
			}
			if (candidates.size() > 1)
			{
				return {invalid_move, "Please provide disambiguation"};
			}
			return {candidates[0].get()->pos, to, capture};
		}
		return {invalid_move, "Unknown notation"};
	}
	auto type = ToType(input[0]);
	if (type == e_empty)
	{
		return {invalid_move, "Invalid piece notation"};
	}
	if (IsFile(input[1]) && input.size() == 4)
	{
		//File disambiguation (Nde5)
		auto to = ToPosition(input.substr(2, 2));
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
					"No " + TypeToString(type) + " on " + std::string(1, input[1]) + " file that can move there"};
		}
		if (candidates.size() > 1)
		{
			return {invalid_move, "Please provide rank disambiguation"};
		}
		return {candidates[0].get()->pos, to, move};
	}
	if (IsRank(input[1]) && input.size() == 4)
	{
		//Rank disambiguation (N1e5)
		auto to = ToPosition(input.substr(2, 2));
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
					"No " + TypeToString(type) + " on " + std::to_string(rank) + " rank that can move there"};
		}
		if (candidates.size() > 1)
		{
			return {invalid_move, "Please provide file disambiguation"};
		}
		return {candidates[0].get()->pos, to, move};
	}

	//No disambiguation (Ne5)
	auto to = ToPosition(input.substr(1, 2));
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
		return {invalid_move, "No " + TypeToString(type) + " on board that can move there"};
	}
	if (candidates.size() > 1)
	{
		return {invalid_move, "Please provide disambiguation"};
	}
	return {candidates[0].get()->pos, to, move};
}

std::vector<std::reference_wrapper<const std::shared_ptr<pieces>>>
board::find_all(type_enum type, enum color_enum color, auto condition) const
{
	std::vector<std::reference_wrapper<const std::shared_ptr<pieces>>> result {};
	for (const auto& piece: val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->type == type && piece->color == color && condition(piece))
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

char type_to_char_FEN(type_enum type, color_enum color)
{
	char k = '?';
	switch (type)
	{
		case e_pawn: k = 'p';
			break;
		case e_king: k = 'k';
			break;
		case e_bishop: k = 'b';
			break;
		case e_rook: k = 'r';
			break;
		case e_knight: k = 'n';
			break;
		case e_queen: k = 'q';
			break;
		default: k = '?';
	}
	if (color == e_white)
	{
		k = toupper(k); // NOLINT(bugprone-narrowing-conversions)
	}
	return k;
}

std::string board::save(color_enum turn, int half_move, int full_move) const
{
	//Generate FEN string
	std::array<std::array<std::pair<type_enum, color_enum>, 8>, 8> pos_board {};
	for (const auto& a: val)
	{
		if (a.get() != nullptr)
		{
			pos_board[a->pos.x][a->pos.y] = std::make_pair(a->type, a->color);
		}
	}
	int accumulative = 0;
	std::string result;
	for (int y = 7; y >= 0; y--)
	{
		for (int x = 0; x < 8; x++)
		{
			auto piece = pos_board[x][y];
			if (piece.first == e_empty)
			{
				accumulative++;
			}
			else
			{
				if (accumulative != 0)
				{
					result += std::to_string(accumulative);
					accumulative = 0;
				}
				result += type_to_char_FEN(piece.first, piece.second);
			}
		}
		if (accumulative != 0)
		{
			result += std::to_string(accumulative);
			accumulative = 0;
		}
		if (y != 0)
		{
			result += '/';
		}
	}
	if (accumulative != 0)
	{
		result += std::to_string(accumulative);
		accumulative = 0;
	}
	result += (turn == e_white ? " w " : " b ");
	//Castling availability
	bool white_k = !find(e_king, e_white)->get()->has_moved;
	bool black_k = !find(e_king, e_black)->get()->has_moved;
	bool white_r_ks = !find_all(e_rook, e_white, [&](auto a)
	{
		return a->pos.x == 7 && a->pos.y == 0 && (!a->has_moved);
	}).empty();
	bool white_r_qs = !find_all(e_rook, e_white, [&](auto a)
	{
		return a->pos.x == 0 && a->pos.y == 0 && (!a->has_moved);
	}).empty();
	bool black_r_ks = !find_all(e_rook, e_black, [&](auto a)
	{
		return a->pos.x == 7 && a->pos.y == 7 && (!a->has_moved);
	}).empty();
	bool black_r_qs = !find_all(e_rook, e_black, [&](auto a)
	{
		return a->pos.x == 0 && a->pos.y == 7 && (!a->has_moved);
	}).empty();
	std::string temp {};
	if (white_k)
	{
		if (white_r_ks)
		{
			temp += 'K';
		}
		if (white_r_qs)
		{
			temp += 'Q';
		}
	}
	if (black_k)
	{
		if (black_r_ks)
		{
			temp += 'k';
		}
		if (black_r_qs)
		{
			temp += 'q';
		}
	}
	if (temp.empty())
	{
		temp = "-";
	}
	result += temp;
	result += ' ';

	//En passant capability
	auto en_passant = find_all(e_pawn, static_cast<color_enum>(!turn), [&](auto a)
	{
		return a->just_moved_2square;
	});
	if (en_passant.empty())
	{
		result += '-';
	}
	else
	{
		auto pos = en_passant[0].get()->pos;
		pos.y += (turn == e_white ? 1 : -1);
		result += static_cast<char>(pos.x + 'a');
		result += static_cast<char>(pos.y + '1');
	}
	result += ' ';
	result += std::to_string(half_move) + ' ' + std::to_string(full_move);
	return {result};
}

std::string board::load(std::string_view input, int& half_move, int& full_move, color_enum& turn)
{
	//Load FEN string
	/*
	 * Update these variable:
	 * board
	 * half_move
	 * full_move
	 * turn
	*/
	std::stringstream ss {(std::string)input};
	std::string pos {}, castling {}, en_passant {};
	char turn_k = 0;
	ss >> pos >> turn_k >> castling >> en_passant >> half_move >> full_move;
	turn = (turn_k == 'w' ? e_white : e_black);
	position ep_pos = ToPosition(en_passant);
	if (en_passant == "-")
	{
		ep_pos = {-1, -1};
	}
	else if (ep_pos.x == -1 || ep_pos.y == -1)
	{
		return "Invalid en passant position";
	}
	ep_pos.y += (turn == e_white ? -1 : 1);
	//Bool to check validity of position
	bool ep_pos_valid = false, w_king = false, b_king = false,
	//Bool to check if both king and rook are present for castle availability
	w_king_c = false, b_king_c = false,
		w_rook_ks = false, w_rook_qs = false, b_rook_ks = false, b_rook_qs = false,
	//Bool for castling
	castle_wks = false, castle_wqs = false, castle_bks = false, castle_bqs = false;
	std::map<std::tuple<type_enum, color_enum>, int> piece_count;
	for (const char& k: castling)
	{
		if (k == 'K')
		{
			castle_wks = true;
		}
		else if (k == 'Q')
		{
			castle_wqs = true;
		}
		else if (k == 'k')
		{
			castle_bks = true;
		}
		else if (k == 'q')
		{
			castle_bqs = true;
		}
		else
		{
			return "Invalid castling availability notation";
		}
	}
	bool en_passant_present = (en_passant != "-");
	//Update board
	int x = 0, y = 7, ind = 0;
	for (const char& k: pos)
	{
		if (k == '/')
		{
			x = 0;
			y--;
		}
		else if (isdigit(k))
		{
			x += k - '0';
		}
		else
		{
			color_enum color = (std::isupper(k) ? e_white : e_black);
			type_enum type = ToType(std::toupper(k)); //NOLINT
			switch (type)
			{
				case e_pawn: val[ind] = std::make_shared<pawn>(position {x, y}, color);
					piece_count[std::make_pair(e_pawn, color)]++;
					break;
				case e_knight: val[ind] = std::make_shared<knight>(position {x, y}, color);
					piece_count[std::make_pair(e_knight, color)]++;
					break;
				case e_bishop: val[ind] = std::make_shared<bishop>(position {x, y}, color);
					piece_count[std::make_pair(e_bishop, color)]++;
					break;
				case e_rook: val[ind] = std::make_shared<rook>(position {x, y}, color);
					piece_count[std::make_pair(e_rook, color)]++;
					break;
				case e_queen: val[ind] = std::make_shared<queen>(position {x, y}, color);
					piece_count[std::make_pair(e_queen, color)]++;
					break;
				case e_king: val[ind] = std::make_shared<king>(position {x, y}, color);
					piece_count[std::make_pair(e_king, color)]++;
					break;
				default: return "Invalid type notation";
			}
			//Check validity of position
			if (type == e_king)
			{
				if (color == e_white && (x != 4 || y != 0) && (castle_wks || castle_wqs))
				{
					return "Conflict between white king position and castling availability";
				}
				if (color == e_black && (x != 4 || y != 7) && (castle_bks || castle_bqs))
				{
					return "Conflict between black king position and castling availability";
				}
				if (color == e_white)
				{
					w_king = true;
					if (((!castle_wks) && (!castle_wqs)) || x != 4 || y != 0)
					{
						val[ind]->has_moved = true;
					}
					else
					{
						w_king_c = true;
					}
				}
				else
				{
					b_king = true;
					if (((!castle_bks) && (!castle_bqs)) || x != 4 || y != 7)
					{
						val[ind]->has_moved = true;
					}
					else
					{
						b_king_c = true;
					}
				}
			}
			if (type == e_rook)
			{
				if (color == e_white)
				{
					if ((x == 0 && y == 0 && !castle_wqs) || (x == 7 && y == 0 && !castle_wks) ||
						((x != 0 || y != 0) && (x != 7 || y != 0)))
					{
						val[ind]->has_moved = true;
					}
					if (x == 0 && y == 0 && castle_wqs)
					{
						w_rook_qs = true;
					}
					if (x == 7 && y == 0 && castle_wks)
					{
						w_rook_ks = true;
					}
				}
				else
				{
					if ((x == 0 && y == 7 && !castle_bqs) || (x == 7 && y == 7 && !castle_bks) ||
						((x != 0 || y != 7) && (x != 7 || y != 7)))
					{
						val[ind]->has_moved = true;
					}
					if (x == 0 && y == 7 && castle_bqs)
					{
						b_rook_qs = true;
					}
					if (x == 7 && y == 7 && castle_bks)
					{
						b_rook_ks = true;
					}
				}
			}
			//Check en passant
			if (type == e_pawn)
			{
				if (en_passant_present && (x == ep_pos.x) && (y == ep_pos.y))
				{
					val[ind]->just_moved_2square = true;
					ep_pos_valid = true;
				}
			}
			ind++;
			x++;
		}
	}
	if (ep_pos_valid != en_passant_present)
	{
		return "No pawn on the provided en passant square";
	}
	if (castle_wqs && (!find({0,0}).has_value() || find({0,0}).value().get()->type != e_rook))
	{
		return "No white queen-side rook while castling availability is set";
	}
	if (castle_wks && (!find({7,0}).has_value() || find({7,0}).value().get()->type != e_rook))
	{
		return "No white king-side rook while castling availability is set";
	}
	if (castle_bqs && (!find({0,7}).has_value() || find({0,7}).value().get()->type != e_rook))
	{
		return "No black queen-side rook while castling availability is set";
	}
	if (castle_bks && (!find({7,7}).has_value() || find({7,7}).value().get()->type != e_rook))
	{
		return "No black king-side rook while castling availability is set";
	}
	if (!w_king)
	{
		return "No white king";
	}
	if (!b_king)
	{
		return "No black king";
	}
	if ((castle_wks || castle_wqs) != w_king_c)
	{
		return "Conflict between white king position and castling availability";
	}
	if ((castle_bks || castle_bqs) != b_king_c)
	{
		return "Conflict between black king position and castling availability";
	}
	if (castle_wks != w_rook_ks)
	{
		return "Conflict between white king-side rook position and castling availability";
	}
	if (castle_wqs != w_rook_qs)
	{
		return "Conflict between white queen-side rook position and castling availability";
	}
	if (castle_bks != b_rook_ks)
	{
		return "Conflict between black king-side rook position and castling availability";
	}
	if (castle_bqs != b_rook_qs)
	{
		return "Conflict between black queen-side rook position and castling availability";
	}

	//Check piece number
	for (int c = 0; c < 2; c++)
	{
		std::string color_str = (c == 0) ? "white" : "black";
		if (piece_count[std::make_pair(e_pawn, (color_enum)c)] > 8)
		{
			return "Invalid number of " + color_str + " pawns";
		}
		if (piece_count[std::make_pair(e_knight, (color_enum)c)] > 2)
		{
			return "Invalid number of " + color_str + " knights";
		}
		if (piece_count[std::make_pair(e_bishop, (color_enum)c)] > 2)
		{
			return "Invalid number of " + color_str + " bishops";
		}
		if (piece_count[std::make_pair(e_rook, (color_enum)c)] > 2)
		{
			return "Invalid number of " + color_str + " rooks";
		}
		if (piece_count[std::make_pair(e_queen, (color_enum)c)] > 1)
		{
			return "Invalid number of " + color_str + " queens";
		}
		if (piece_count[std::make_pair(e_king, (color_enum)c)] > 1)
		{
			return "Invalid number of " + color_str + " kings";
		}
	}
	return {};
}