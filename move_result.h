//
// Created by Admin on 31/03/2023.
//

#ifndef CHESS_MOVE_RESULT_H
#define CHESS_MOVE_RESULT_H


#include <string>
#include <utility>
#include <iostream>
#include "enum.h"

class move_result
{
public:
    move_state state;
    std::string message{};

    explicit move_result(const move_state _s) : state(_s)
    {};

    move_result(const move_state _s, const std::string &mes) : state(_s), message(mes)
    {};

    move_result(const move_state _s, std::string &&mes) : state(_s), message(std::move(mes))
    {};

    static std::string type_to_string (type_enum type);
};


#endif //CHESS_MOVE_RESULT_H