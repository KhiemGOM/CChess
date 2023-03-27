//
// Created by Admin on 25/03/2023.
//

#include "pieces.h"
#include "board.h"

using namespace std::rel_ops;

class board;

color_enum invert(color_enum _color) {
    if (_color == e_white)
        return e_black;
    return e_white;
}

move_state pieces::move(board &game_board, position target) {
    if (is_within_field(target) && target != pos) {
        auto piece_at_target = game_board.find(target);
        if (piece_at_target.has_value()) {
            if (is_valid_capture(game_board, target) && piece_at_target.value().get()->color != color) {
                //Check if the king is in check
                if (is_being_checked(game_board, target, color))
                    return move_state::invalid_move;
                pos = target;
                piece_at_target.value().get() = nullptr;
                if (is_check(game_board, invert(color)))
                    return move_state::check;
                return move_state::valid_capture;
            }
            return move_state::invalid_move;
        }
        if (is_valid_move(game_board, target)) {
            //Check if the king is in check
            if (is_being_checked(game_board, target, color))
                return move_state::invalid_move;
            pos = target;
            if (is_check(game_board, invert(color)))
                return move_state::check;
            return move_state::valid_move;
        }
        return move_state::invalid_move;
    }
    return move_state::invalid_move;
}

move_state pieces::try_to_move(board &game_board, position target) {
    if (is_within_field(target) && target != pos) {
        const auto piece_at_target = game_board.find(target);
        if (piece_at_target.has_value()) {
            if (is_valid_capture(game_board, target) && piece_at_target.value().get()->color != color) {
                //Check if the king is in check
                if (is_being_checked(game_board, target, color))
                    return move_state::invalid_move;
                if (is_check(game_board, invert(color)))
                    return move_state::check;
                return move_state::valid_capture;
            }
            return move_state::invalid_move;
        }
        if (is_valid_move(game_board, target)) {
            //Check if the king is in check
            if (is_being_checked(game_board, target, color))
                return move_state::invalid_move;
            if (is_check(game_board, invert(color)))
                return move_state::check;
            return move_state::valid_move;
        }
        return move_state::invalid_move;
    }
    return move_state::invalid_move;
}

bool pieces::is_within_field(position pos) {
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

bool pieces::is_check(board &game_board, color_enum _color) {
    return is_valid_move(game_board, game_board.find(e_king, _color).value().get()->pos);
}

bool pieces::is_being_checked(board game_board_copy, position target, color_enum _color) const {
    std::shared_ptr<pieces> piece_copy = std::make_shared<pieces>(*this);
    piece_copy->pos = target;
    for (auto &piece: game_board_copy.val) {
        if (piece != nullptr) {
            if (piece->color != _color) {
                if (piece->is_check(game_board_copy, _color)) {
                    piece_copy->pos = this->pos;
                    return true;
                }
            }
        }
    }
    piece_copy->pos = this->pos;
    return false;
}

bool pieces::is_valid_move(board &game_board, position target) {
    return false;
}

bool pieces::is_valid_capture(board &game_board, position target) {
    return false;
}

bool pieces::is_obstruct(board &game_board, position target) {
    return false;
}