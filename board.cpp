//
// Created by Admin on 25/03/2023.
//

#include <functional>
#include <memory>
#include <optional>
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

std::optional<std::reference_wrapper<const std::shared_ptr<pieces>>> board::find(type_enum type, enum color_enum color) const
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
			auto temp = val[i]->clone();
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
	std::shared_ptr<pieces>& piece_to_promote = find(
			piece->pos).value().get(); // NOLINT(bugprone-unchecked-optional-access)
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
		if (val[i].get() != nullptr)
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