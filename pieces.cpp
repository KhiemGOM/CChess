//
// Created by Admin on 25/03/2023.
//

#include "pieces.h"
#include "board.h"
#include "enum.h"
#include "move_result.h"
#include <string>

using namespace std::rel_ops;

class board;

color_enum Invert(color_enum color)
{
	if (color == e_white)
	{
		return e_black;
	}
	return e_white;
}

move_result pieces::move(board& game_board, position target, type_enum promotion_type)
{
	move_state ret {};
	if (!is_within_field(target))
	{
		return {{invalid_move}, "Target square is not on the board"};
	}
	if (target == pos)
	{
		return {{invalid_move}, "A piece cannot move to the square it's standing on"};
	}

	//There's piece at target
	std::shared_ptr<pieces> shared_ptr_of_this = game_board.find( // NOLINT(bugprone-unchecked-optional-access)
		{pos.x, pos.y}).value().get();
	auto find_result = game_board.find(target);
	if (find_result.has_value())
	{
		//Check for castle
		std::shared_ptr<pieces>& piece_at_target = find_result.value().get();
		if (((type == e_king && piece_at_target->type == e_rook) ||
			 (type == e_rook && piece_at_target->type == e_king)) && color == piece_at_target->color)
		{
			if (has_moved || piece_at_target->has_moved)
			{
				return {{invalid_move}, "King or rook has already moved"};
			}
			auto rook_temp = type == e_king ? piece_at_target : shared_ptr_of_this;
			//King side
			if (pos.x == 7 || piece_at_target->pos.x == 7)
			{
				ret.states.push_back(ks_castle);
				switch (type)
				{
					case e_king:
						if (!game_board.able_to_castle(shared_ptr_of_this, piece_at_target))
						{
							return {{invalid_move}, "Isn't able to castle"};
						}
						pos.x = 6;
						piece_at_target->pos.x = 5;
						break;
					case e_rook:
						if (!game_board.able_to_castle(piece_at_target, shared_ptr_of_this))
						{
							return {{invalid_move}, "Isn't able to castle"};
						}
						pos.x = 5;
						piece_at_target->pos.x = 6;
					default: break;
				}
			}
			//Queen side
			if (pos.x == 0 || piece_at_target->pos.x == 0)
			{
				ret.states.push_back(qs_castle);
				switch (type)
				{
					case e_king:
						if (!game_board.able_to_castle(shared_ptr_of_this, piece_at_target))
						{
							return {{invalid_move}, "Isn't able to castle"};
						}
						pos.x = 2;
						piece_at_target->pos.x = 3;
						break;
					case e_rook:
						if (!game_board.able_to_castle(piece_at_target, shared_ptr_of_this))
						{
							return {{invalid_move}, "Isn't able to castle"};
						}
						pos.x = 3;
						piece_at_target->pos.x = 2;
					default: break;
				}
			}
			if (rook_temp->is_check(game_board, Invert(color)))
			{
				ret.states.push_back(check);
			}
			return move_result {ret};
		}
		if (piece_at_target->type == e_king)
		{
			return {{invalid_move}, "Cannot capture a king"};
		}
		if (is_valid_capture(game_board, target) && piece_at_target->color != color)
		{
			//Check if the king is in check
			if (is_being_checked_after_move(game_board, target, color))
			{
				if (game_board.is_being_checked(color))
				{
					return {{invalid_move}, "Your king is in check"};
				}
				return {{invalid_move}, "Your capture results in a check"};
			}
			if (type == e_king || type == e_rook || (type == e_pawn && abs(pos.y - target.y) == 2))
			{
				move_cleanup();
			}
			pos = target;
			find_result.value().get() = nullptr;
			//Check for promotion
			if ((type == e_pawn && color == e_white && target.y == 7) ||
				(type == e_pawn && color == e_black && target.y == 0))
			{
				if (promotion_type != e_empty)
				{
					game_board.promote(shared_ptr_of_this, target, color, promotion_type);
				}
				else
				{
					//Change type
					if (!game_board.promote_input(shared_ptr_of_this, target, color))
					{
						return move_result {{invalid_move}, "Invalid input for promotion"};
					}
					if (is_check(game_board, Invert(color)))
					{
						return move_result {{check, promotion, capture}};
					}
					return move_result {{promotion, capture}};
				}
			}
			if (is_check(game_board, Invert(color)))
			{
				return move_result {{check, capture}};
			}
			return move_result {{capture}};
		}
		return {{invalid_move},
				"Your " + move_result::type_to_string(this->type) + " can't capture the piece at " +
				std::string(1, target.x + 'a') + // NOLINT(bugprone-narrowing-conversions)
				std::to_string(target.y + 1)};
	}

	//No piece at target
	if (type == e_pawn)
	{
		//Check for en passant
		if (color == e_white && pos.y == 4 && target.y == 5)
		{
			auto target_pawn = game_board.find({target.x, 4});
			if (target_pawn.has_value())
			{
				if (target_pawn.value().get()->just_moved_2square && target_pawn.value().get()->color == e_black)
				{
					pos = {target.x, 5};
					target_pawn.value().get() = nullptr;
					if (is_check(game_board, Invert(color)))
					{
						return move_result {{check, capture}};
					}
					return move_result {{capture}};
				}
			}
		}
		if (color == e_black && pos.y == 3 && target.y == 2)
		{
			auto target_pawn = game_board.find({target.x, 3});
			if (target_pawn.has_value())
			{
				if (target_pawn.value().get()->just_moved_2square && target_pawn.value().get()->color == e_white)
				{
					pos = {target.x, 2};
					target_pawn.value().get() = nullptr;
					if (is_check(game_board, Invert(color)))
					{
						return move_result {{check, capture}};
					}
					return move_result {{capture}};
				}
			}
		}
	}

	//Normal move
	if (is_valid_move(game_board, target))
	{
		//Check if the king is in check
		if (is_being_checked_after_move(game_board, target, color))
		{
			if (game_board.is_being_checked(color))
			{
				return {{invalid_move}, "Your king is in check"};
			}
			return {{invalid_move}, "Your move results in a check"};
		}
		if (type == e_king || type == e_rook || (type == e_pawn && abs(pos.y - target.y) == 2))
		{
			move_cleanup();
		}
		pos = target;

		//Check for promotion
		if ((type == e_pawn && color == e_white && target.y == 7) ||
			(type == e_pawn && color == e_black && target.y == 0))
		{
			if (promotion_type != e_empty)
			{
				game_board.promote(shared_ptr_of_this, target, color, promotion_type);
			}
			else
			{
				//Change type
				if (!game_board.promote_input(shared_ptr_of_this, target, color))
				{
					return move_result {{invalid_move}, "Invalid input for promotion"};
				}
				if (is_check(game_board, Invert(color)))
				{
					return move_result {{check, promotion}};
				}
				return move_result {{promotion}};
			}
		}
		if (is_check(game_board, Invert(color)))
		{
			return move_result {{check}};
		}
		return move_result {{_move_state::move}};
	}
	return {{invalid_move},
			"Your " + move_result::type_to_string(this->type) + " can't move to " + std::string(1, target.x + 'a') +
			// NOLINT(bugprone-narrowing-conversions)
			// NOLINT(bugprone-narrowing-conversions)
			std::to_string(target.y + 1)};
}

move_state pieces::try_to_move(board& game_board, position target, type_enum promotion_type) const
{
	auto *copy = this->clone();
	return copy->move(game_board, target, promotion_type).state;
}

bool pieces::is_within_field(position pos)
{
	return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

/**
 * The color is for the opposite color (color of the king)
 **/
bool pieces::is_check(board& game_board, color_enum current_color) const
{
	auto king_temp = game_board.find(e_king, current_color);
	if (!king_temp.has_value())
	{
		return false;
	}
	return is_valid_capture(game_board, king_temp.value().get()->pos);
}

/**
 * The color is for the color of side wanting to check if their move results in a check
 **/
bool pieces::is_being_checked_after_move(const board& game_board, position target, color_enum current_color) const
{
	//Check if any pieces of opposite color is checking the king
	//after the move on a copied version of the game board
	auto game_board_copy = game_board.clone();
	const auto find_result = game_board_copy.find(target);

	if (find_result.has_value())
	{
		auto& piece_at_target = find_result.value().get();
		if (piece_at_target->type != e_king)
		{
			piece_at_target = nullptr;
		}
		else
		{
			return true;
		}
	}
	game_board_copy.find(pos).value().get()->pos = target;

	for (auto& piece: game_board_copy.val)
	{
		if (piece.get() != nullptr)
		{
			if (piece->color != current_color)
			{
				if (piece->is_check(game_board_copy, current_color))
				{
					return true;
				}
			}
		}
	}
	return false;
}

color_enum pieces::square_color() const
{
	return (pos.x % 2 != pos.y % 2) ? e_white : e_black;
}