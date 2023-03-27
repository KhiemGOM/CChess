#include <iostream>
#include <map>
#include <sstream>
#include "pieces.h"
#include "board.h"
#include "king.h"
#include "queen.h"
#include "pawn.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "enum.h"
#include "Windows.h"

void displayBoard(std::map<type_enum, char> &type_to_char, board &game_board);

color_enum invert(color_enum _color);

int main() {
    std::map<type_enum, char> type_to_char{
            {e_king,   'K'},
            {e_queen,  'Q'},
            {e_pawn,   'P'},
            {e_rook,   'R'},
            {e_bishop, 'B'},
            {e_knight, 'N'},
            {e_empty,  ' '}
    };
    board game_board{std::make_shared<king>(position{4, 0}, color_enum::e_white),
                     std::make_shared<king>(position{4, 7}, color_enum::e_black),
                     std::make_shared<queen>(position{3, 0}, color_enum::e_white),
                     std::make_shared<queen>(position{3, 7}, color_enum::e_black),
                     std::make_shared<pawn>(position{0, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{1, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{2, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{3, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{4, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{5, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{6, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{7, 1}, color_enum::e_white),
                     std::make_shared<pawn>(position{0, 6}, color_enum::e_black),
                     std::make_shared<pawn>(position{1, 6}, color_enum::e_black),
                     std::make_shared<pawn>(position{2, 6}, color_enum::e_black),
                     std::make_shared<pawn>(position{3, 6}, color_enum::e_black),
                     std::make_shared<pawn>(position{4, 6}, color_enum::e_black),
                     std::make_shared<pawn>(position{5, 6}, color_enum::e_black),
                     std::make_shared<pawn>(position{6, 6}, color_enum::e_black),
                     std::make_shared<pawn>(position{7, 6}, color_enum::e_black),
                     std::make_shared<rook>(position{0, 0}, color_enum::e_white),
                     std::make_shared<rook>(position{7, 0}, color_enum::e_white),
                     std::make_shared<rook>(position{0, 7}, color_enum::e_black),
                     std::make_shared<rook>(position{7, 7}, color_enum::e_black),
                     std::make_shared<bishop>(position{2, 0}, color_enum::e_white),
                     std::make_shared<bishop>(position{5, 0}, color_enum::e_white),
                     std::make_shared<bishop>(position{2, 7}, color_enum::e_black),
                     std::make_shared<bishop>(position{5, 7}, color_enum::e_black),
                     std::make_shared<knight>(position{1, 0}, color_enum::e_white),
                     std::make_shared<knight>(position{6, 0}, color_enum::e_white),
                     std::make_shared<knight>(position{1, 7}, color_enum::e_black),
                     std::make_shared<knight>(position{6, 7}, color_enum::e_black)};
    color_enum turn = e_white;
    while (!(GetKeyState(VK_ESCAPE) & 0x8000)) {
        //Display the pieces on the board
        displayBoard(type_to_char, game_board);
        std::string input{}, word;
        int xi, yi, xf, yf;
        move_state move = invalid_move;
        while (move == invalid_move) {
            std::cout << "Enter a move(xi + yi + xf + yf/exit to stop program): ";
            std::getline(std::cin, input);
            if (input == "exit") {
                return 0;
            }
            std::stringstream ss{input};
            ss >> xi >> yi >> xf >> yf;
            xi--;
            yi--;
            xf--;
            yf--;
            auto r = game_board.find(position{xi, yi});
            if (r.has_value()) {
                if (r.value().get()->color == turn) {
                    move = r.value().get()->move(game_board, position{xf, yf});
                    if (move != invalid_move) {
                        std::cout << "Moved" << std::endl;
                        if (game_board.is_out_of_moves(invert(turn))) {
                            if (move == check) {
                                std::cout << std::string("Checkmate, ").append(turn == e_white ? "white" : "black") +
                                             "won" << std::endl;
                                return 0;
                            } else {
                                std::cout << "Stalemate, draw" << std::endl;
                                return 0;
                            }
                        }
                        turn = invert(turn);
                    } else {
                        std::cout << "Invalid move: Can't move piece like that" << std::endl;
                    }
                } else {
                    std::cout << "Invalid move: It's not their turn yet" << std::endl;
                }
            } else {
                std::cout << "Invalid move: No piece on " + std::to_string(xi+1) + " " + std::to_string(yi+1) << std::endl;
            }
        }
    }
}

void displayBoard(std::map<type_enum, char> &type_to_char, board &game_board) {
    std::array<std::array<char, 8>, 8> pos_board{};
    for (auto &a: game_board.val) {
        if (a.get() != nullptr) {
            pos_board[7 - a->pos.y][a->pos.x] = type_to_char[a->type];
        }
    }
    for (auto &a: pos_board) {
        for (auto &b: a) {
            std::cout << b << " ";
        }
        std::cout << std::endl;
    }
}