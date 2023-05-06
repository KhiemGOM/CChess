//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_ENUM_H
#define CHESS_ENUM_H

enum color_enum
{
	e_black,
	e_white
};
enum type_enum
{
	e_empty, e_pawn, e_rook, e_knight, e_bishop, e_queen, e_king
};
enum move_state
{
	move,
	capture,
	invalid_move,
	check,
	check_capture,
	promotion,
	check_promotion
};

#endif //CHESS_ENUM_H