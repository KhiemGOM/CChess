//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_COLOR_ENUM_H
#define CHESS_COLOR_ENUM_H

#include "pieces.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"

enum color_enum {
    e_black,
    e_white
};

enum type_enum {
    e_pawn, e_rook, e_knight, e_bishop, e_queen, e_king, e_empty
};

enum move_state {
    valid_move,
    valid_capture,
    invalid_move,
    check,
    end_game
};

#endif //CHESS_COLOR_ENUM_H