//
// Created by Admin on 13/06/2023.
//

#ifndef CHESS_ULTILITIES_H
#define CHESS_ULTILITIES_H
#include <enum.h>
//General utility functions
char type_to_upper_char(type_enum type)
{
	switch (type)
	{
		case e_empty: return ' ';
		case e_pawn: return 'P';
		case e_rook: return 'R';
		case e_knight: return 'N';
		case e_bishop: return 'B';
		case e_queen: return 'Q';
		case e_king: return 'K';
		default: return '?';
	}
}

#endif //CHESS_ULTILITIES_H