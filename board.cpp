//
// Created by Admin on 25/03/2023.
//

#include <optional>
#include <iostream>
#include "board.h"
#include "king.h"

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
    for (int i = 0; i < list.size(); i++)
        val[i] = *(list.begin() + i);
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

bool board::able_to_castle(const std::shared_ptr<pieces> &king_o, const std::shared_ptr<pieces> &rook_o) const {
    auto copy_board = clone();
    std::weak_ptr<pieces> king_ptr = std::weak_ptr<pieces>{copy_board.find(king_o->pos).value().get()};
    std::weak_ptr<pieces> rook_ptr = std::weak_ptr<pieces>{copy_board.find(rook_o->pos).value().get()};
    //Check if king in check
    for (auto &piece: copy_board.val) {
        if (piece.get() != nullptr) {
            if (piece->color != king_ptr.lock()->color) {
                if (piece->is_valid_capture(copy_board, king_ptr.lock()->pos))
                    return false;
            }
        }
    }
    //Check between the king_ptr and the rook_ptr if the space is occupied or is under controlled by another pieces of opposite color
    if (king_ptr.lock()->pos.x < rook_ptr.lock()->pos.x) {
        for (int i = king_ptr.lock()->pos.x + 1; i < rook_ptr.lock()->pos.x; i++) {
            if (copy_board.find({i, king_ptr.lock()->pos.y}).has_value())
                return false;
            for (auto &piece: copy_board.val) {
                if (piece.get() != nullptr) {
                    if (piece->color != king_ptr.lock()->color) {
                        if (piece->is_valid_capture(copy_board, {i, king_ptr.lock()->pos.y}))
                            return false;
                    }
                }
            }
        }
    } else {
        for (int i = rook_ptr.lock()->pos.x + 1; i < king_ptr.lock()->pos.x; i++) {
            if (copy_board.find({i, king_ptr.lock()->pos.y}).has_value())
                return false;
            for (auto &piece: copy_board.val) {
                if (piece.get() != nullptr) {
                    if (piece->color != king_ptr.lock()->color) {
                        if (piece->is_valid_capture(copy_board, {i, king_ptr.lock()->pos.y}))
                            return false;
                    }
                }
            }
        }
    }
    return true;
}