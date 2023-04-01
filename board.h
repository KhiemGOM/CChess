//
// Created by Admin on 25/03/2023.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>
#include <array>
#include <memory>
#include "pieces.h"
#include <optional>
#include "enum.h"

class pieces;

class position;

class board {
    static const int NUM_OF_PIECES = 32;
public:
    std::array<std::shared_ptr<pieces>, NUM_OF_PIECES> val;

    board(board& b) = default;

    board() = default;

    board(std::initializer_list<std::shared_ptr<pieces>> list);;

    [[nodiscard]] std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> find(position pos);

    [[nodiscard]] std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> find(type_enum type, enum color_enum color);

    [[nodiscard]] bool is_out_of_moves(color_enum _color) const;

    [[nodiscard]] board& clone() const;

    [[nodiscard]] bool able_to_castle (const std::shared_ptr<pieces>& king, const std::shared_ptr<pieces>& rook) const;

    [[nodiscard]] bool is_being_checked (color_enum _color);
};


#endif //CHESS_BOARD_H