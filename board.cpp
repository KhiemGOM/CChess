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
            auto temp = val[i]->clone();
            b->val[i].reset(temp);
        }
    }
    return *b;
}

bool board::able_to_castle(std::shared_ptr<pieces> &king, std::shared_ptr<pieces> &rook) const {
    auto copy_board = clone();

}