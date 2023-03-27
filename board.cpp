//
// Created by Admin on 25/03/2023.
//

#include <optional>
#include "board.h"

std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> board::find(position pos) {
    for (auto &piece: val) {
        if (piece.get() != nullptr) {
            if (piece->pos.x == pos.x && piece->pos.y == pos.y) {
                return {piece};
            }
        }
    }
    return {};
}

std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> board::find(type_enum _type, enum color_enum _color) {
    for (auto &piece: val) {
        if (piece.get() != nullptr) {
            if (piece->type == _type && piece->color == _color) {
                return {piece};
            }
        }
    }
    return {};
}

bool board::is_out_of_moves(color_enum _color) const {
    //Check if all pieces are out of moves
    auto t = clone();
    for (auto &piece: t.val) {
        if (piece.get() != nullptr) {
            if (piece->color == _color) {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        if (piece->try_to_move(t, {i, j}) != move_state::invalid_move)
                            return false;
                    }
                }
            }
        }
    }
    return true;
}

board::board(std::initializer_list<std::shared_ptr<pieces>> list) {
    for(int i=0; i<NUM_OF_PIECES; i++)
        val[i] = *(list.begin()+i);
}

board &board::clone() const {
    auto *b = new board();
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        if (val[i].get() != nullptr) {
            b->val[i] = std::dynamic_pointer_cast<typeof(*val[i].get())>(std::make_shared<pieces>(*(val[i].get())));
        }
    }
    return *b;
}

template <typename T>
auto pieces_casting(pieces& piece)
{
    T = typeof(piece);
    return std::dynamic_pointer_cast<T>(std::make_shared<pieces>(piece));
}