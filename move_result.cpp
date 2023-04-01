//
// Created by Admin on 31/03/2023.
//

#include "move_result.h"
#include "enum.h"

std::string move_result::type_to_string(type_enum type)
{
    switch (type)
    {
        case (e_king):return "king";
        case (e_rook):return "rook";
        case (e_bishop):return "bishop";
        case (e_knight):return "knight";
        case (e_queen):return "queen";
        case (e_pawn):return "pawn";
        case e_empty:break;
    }
    return "empty";
}