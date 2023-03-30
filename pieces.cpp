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
        auto find_result = game_board.find(target);
        if (find_result.has_value()) {
            //Check for castle
            std::shared_ptr<pieces> &piece_at_target = find_result.value().get();
            if (((type == e_king && piece_at_target->type == e_rook) ||
                 (type == e_rook && piece_at_target->type == e_king)) &&
                color == piece_at_target->color &&
                !has_moved && !piece_at_target->has_moved) {
                auto shared_ptr_of_this = game_board.find({pos.x, pos.y}).value().get();
                //King side
                if (pos.x == 7 || piece_at_target->pos.x == 7) {
                    switch (type) {
                        case e_king:
                            if (!game_board.able_to_castle(shared_ptr_of_this, piece_at_target))
                                return move_state::invalid_move;
                            pos.x = 5;
                            piece_at_target->pos.x = 6;
                            break;
                        case e_rook:
                            if (!game_board.able_to_castle(piece_at_target, shared_ptr_of_this))
                                return move_state::invalid_move;
                            pos.x = 6;
                            piece_at_target->pos.x = 5;
                        default:
                            break;
                    }
                }
                //Queen side
                if (pos.x == 0 || piece_at_target->pos.x == 0) {
                    switch (type) {
                        case e_king:
                            if (!game_board.able_to_castle(shared_ptr_of_this, piece_at_target))
                                return move_state::invalid_move;
                            pos.x = 2;
                            piece_at_target->pos.x = 3;
                            break;
                        case e_rook:
                            if (!game_board.able_to_castle(piece_at_target, shared_ptr_of_this))
                                return move_state::invalid_move;
                            pos.x = 3;
                            piece_at_target->pos.x = 2;
                        default:
                            break;
                    }
                }
                return move_state::valid_move;
            }
            if (piece_at_target->type == e_king) {
                return move_state::invalid_move;
            }
            if (is_valid_capture(game_board, target) && piece_at_target->color != color) {
                //Check if the king is in check
                if (is_being_checked(game_board, target, color))
                    return move_state::invalid_move;
                if (type == e_king || type == e_rook || (type == e_pawn && abs(pos.y - target.y) == 2)) {
                    move_cleanup();
                }
                pos = target;
                find_result.value().get() = nullptr;
                if (is_check(game_board, invert(color)))
                    return move_state::check;
                return move_state::valid_capture;
            }
            return move_state::invalid_move;
        }
        //Check for en passant
        if (type == e_pawn) {
            if (color == e_white && pos.y == 4 && target.y == 5) {
                auto target_pawn = game_board.find({target.x, 4});
                if (target_pawn.has_value()) {
                    if (target_pawn.value().get()->just_moved_2square && target_pawn.value().get()->color == e_black) {
                        pos = {target.x, 5};
                        target_pawn.value().get() = nullptr;
                        return move_state::valid_capture;
                    }
                }
            }
            if (color == e_black && pos.y == 3 && target.y == 2) {
                auto target_pawn = game_board.find({target.x, 3});
                if (target_pawn.has_value()) {
                    if (target_pawn.value().get()->just_moved_2square && target_pawn.value().get()->color == e_white) {
                        pos = {target.x, 2};
                        target_pawn.value().get() = nullptr;
                        return move_state::valid_capture;
                    }
                }
            }
        }
        if (is_valid_move(game_board, target)) {
            //Check if the king is in check
            if (is_being_checked(game_board, target, color))
                return move_state::invalid_move;
            if (type == e_king || type == e_rook || (type == e_pawn && abs(pos.y - target.y) == 2)) {
                move_cleanup();
            }
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
        const auto find_result = game_board.find(target);
        if (find_result.has_value()) {
            //Check for castle
            std::shared_ptr<pieces> &piece_at_target = find_result.value().get();
            if (((type == e_king && piece_at_target->type == e_rook) ||
                 (type == e_rook && piece_at_target->type == e_king)) &&
                color == piece_at_target->color &&
                !has_moved && !piece_at_target->has_moved) {
                auto shared_ptr_of_this = game_board.find({pos.x, pos.y}).value().get();
                //King side
                if (pos.x == 7 || piece_at_target->pos.x == 7) {
                    switch (type) {
                        case e_king:
                            if (!game_board.able_to_castle(shared_ptr_of_this, piece_at_target))
                                return move_state::invalid_move;
                            break;
                        case e_rook:
                            if (!game_board.able_to_castle(piece_at_target, shared_ptr_of_this))
                                return move_state::invalid_move;
                        default:
                            break;
                    }
                }
                //Queen side
                if (pos.x == 0 || piece_at_target->pos.x == 0) {
                    switch (type) {
                        case e_king:
                            if (!game_board.able_to_castle(shared_ptr_of_this, piece_at_target))
                                return move_state::invalid_move;
                            break;
                        case e_rook:
                            if (!game_board.able_to_castle(piece_at_target, shared_ptr_of_this))
                                return move_state::invalid_move;
                        default:
                            break;
                    }
                }
                return move_state::valid_move;
            }
            if (piece_at_target->type == e_king) {
                return move_state::invalid_move;
            }
            if (is_valid_capture(game_board, target) && piece_at_target->color != color) {
                //Check if the king is in check
                if (is_being_checked(game_board, target, color))
                    return move_state::invalid_move;
                if (is_check(game_board, invert(color)))
                    return move_state::check;
                return move_state::valid_capture;
            }
            return move_state::invalid_move;

        }
        //Check for en passant
        if (type == e_pawn) {
            if (color == e_white && pos.y == 4 && target.y == 5) {
                auto target_pawn = game_board.find({target.x, 4});
                if (target_pawn.has_value()) {
                    if (target_pawn.value().get()->just_moved_2square && target_pawn.value().get()->color == e_black) {
                        return move_state::valid_capture;
                    }
                }
            }
            if (color == e_black && pos.y == 3 && target.y == 2) {
                auto target_pawn = game_board.find({target.x, 3});
                if (target_pawn.has_value()) {
                    if (target_pawn.value().get()->just_moved_2square && target_pawn.value().get()->color == e_white) {
                        return move_state::valid_capture;
                    }
                }
            }
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

/**
 * The color is for the opposite color (color of the king)
 **/
bool pieces::is_check(board &game_board, color_enum _color) {
    return is_valid_capture(game_board, game_board.find(e_king, _color).value().get()->pos);
}

/**
 * The color is for the color of side wanting to check if their move results in a check
 **/
bool pieces::is_being_checked(const board &game_board, position target, color_enum _color) const {
    //Check if any pieces of opposite color is checking the king
    //after the move on a copied version of the game board
    auto game_board_copy = game_board.clone();
    const auto find_result = game_board_copy.find(target);
    if (find_result.has_value()) {
        if (find_result->get()->type != e_king) {
            find_result.value().get() = nullptr;
        }
    }
    game_board_copy.find(pos).value().get()->pos = target;

    for (auto &piece: game_board_copy.val) {
        if (piece.get() != nullptr) {
            if (piece->color != _color) {
                if (piece->is_check(game_board_copy, _color))
                    return true;
            }
        }
    }
    return false;
}