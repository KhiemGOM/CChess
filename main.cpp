#include <iostream>
#include <map>
#include <processenv.h>
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
#include <string>
#include <wincon.h>
#include "move_result.h"

int g_c_normal = 7, g_c_white_piece = 15, g_c_black_piece = 0, g_c_white_tile = 112, g_c_black_tile = 160, g_c_normal_tile = 0;

void DISPLAY_END_GAME(std::map<type_enum, char>& type_to_char, board& game_board, std::string&& reason);

void DISPLAY_BOARD(std::map<type_enum, char>& type_to_char, board& game_board);

color_enum INVERT(color_enum color);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "modernize-use-trailing-return-type"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"

//TODO: Checking for (Possibly insufficient material)
//TODO: Standard chess move input (maybe)
int main()
{
	std::map<type_enum, char> type_to_char {{e_king,   'K'},
											{e_queen,  'Q'},
											{e_pawn,   'P'},
											{e_rook,   'R'},
											{e_bishop, 'B'},
											{e_knight, 'N'},
											{e_empty,  ' '}};
	board game_board {std::make_shared<king>(position {4, 0}, color_enum::e_white),
					  std::make_shared<king>(position {4, 7}, color_enum::e_black),
					  std::make_shared<queen>(position {3, 0}, color_enum::e_white),
					  std::make_shared<queen>(position {3, 7}, color_enum::e_black),
					  std::make_shared<pawn>(position {0, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {1, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {2, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {3, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {4, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {5, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {6, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {7, 1}, color_enum::e_white),
					  std::make_shared<pawn>(position {0, 6}, color_enum::e_black),
					  std::make_shared<pawn>(position {1, 6}, color_enum::e_black),
					  std::make_shared<pawn>(position {2, 6}, color_enum::e_black),
					  std::make_shared<pawn>(position {3, 6}, color_enum::e_black),
					  std::make_shared<pawn>(position {4, 6}, color_enum::e_black),
					  std::make_shared<pawn>(position {5, 6}, color_enum::e_black),
					  std::make_shared<pawn>(position {6, 6}, color_enum::e_black),
					  std::make_shared<pawn>(position {7, 6}, color_enum::e_black),
					  std::make_shared<rook>(position {0, 0}, color_enum::e_white),
					  std::make_shared<rook>(position {7, 0}, color_enum::e_white),
					  std::make_shared<rook>(position {0, 7}, color_enum::e_black),
					  std::make_shared<rook>(position {7, 7}, color_enum::e_black),
					  std::make_shared<bishop>(position {2, 0}, color_enum::e_white),
					  std::make_shared<bishop>(position {5, 0}, color_enum::e_white),
					  std::make_shared<bishop>(position {2, 7}, color_enum::e_black),
					  std::make_shared<bishop>(position {5, 7}, color_enum::e_black),
					  std::make_shared<knight>(position {1, 0}, color_enum::e_white),
					  std::make_shared<knight>(position {6, 0}, color_enum::e_white),
					  std::make_shared<knight>(position {1, 7}, color_enum::e_black),
					  std::make_shared<knight>(position {6, 7}, color_enum::e_black)};
	//Set the game_board to a simpler position for checkmate testing
//	game_board = board {std::make_shared<king>(position {6,5}, e_white),
//						std::make_shared<king>(position {7,7}, e_black),
//						std::make_shared<queen>(position {0,6}, e_white),
//						std::make_shared<queen>(position {3,6}, e_black)};

	std::vector<std::pair<board, color_enum>> game_history {};
	color_enum turn = e_white;
	while (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
		move_result move {invalid_move};
		std::string input {}, word, message;
		float move_count = 0.0F;
		while (move.state == invalid_move)
		{
			//Display the pieces on the board
			system("cls");
			HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(h_console, g_c_normal);
			if (!message.empty())
			{
				std::cout << message << '\n';
				message.clear();
			}
			DISPLAY_BOARD(type_to_char, game_board);
			int xi
					{}, yi {}, xf {}, yf {};
			std::cout << "Enter a move(the coordinate (x (in letters), y (in numbers)) of the starting square then the coordinate of the target square)/exit to stop program/config to change color): ";
			std::getline(std::cin, input);
			if (input == "exit")
			{
				return 0;
			}
			if (input == "config")
			{
				system("cls");
				std::cout << "Demo Color:\n";
				for (int i = 0; i < 16; i++)
				{
					SetConsoleTextAttribute(h_console, i);
					std::cout << i << ": " << "Hello World! ";
					SetConsoleTextAttribute(h_console, i + 240);
					std::cout << i << ": " << "Hello World!";
					SetConsoleTextAttribute(h_console, 0);
					std::cout << '\n';
				}
				SetConsoleTextAttribute(h_console, g_c_normal + g_c_normal_tile);
				std::cout
						<< "Input your preferred front color (for colors of text, "
						   "white piece, black piece in that order): ";
				std::string input2;
				getline(std::cin, input2);
				std::stringstream ss2 {input2};
				ss2 >> g_c_normal >> g_c_white_piece >> g_c_black_piece;
				system("cls");
				//Input background color
				for (int i = 0; i < 16; i++)
				{
					SetConsoleTextAttribute(h_console, i * 16);
					std::cout << i << ": " << "Hello World! ";
					SetConsoleTextAttribute(h_console, i * 16 + 15);
					std::cout << i << ": " << "Hello World!";
					SetConsoleTextAttribute(h_console, 0);
					std::cout << '\n';
				}
				SetConsoleTextAttribute(h_console, g_c_normal + g_c_normal_tile);
				std::cout
						<< "Input your preferred background color (for background colors of text, "
						   "white piece, black piece in that order): ";
				getline(std::cin, input2);
				ss2.clear();
				ss2 = std::stringstream {input2};
				ss2 >> g_c_normal_tile >> g_c_white_tile >> g_c_black_tile;
				g_c_normal_tile *= 16;
				g_c_white_tile *= 16;
				g_c_black_tile *= 16;
				system("cls");
				continue;
			}
			//If the first and 3rd word are character, convert them to numbers
			std::stringstream ss {input};
			char fst {}, snd {}, trd {}, fth {};
			ss >> fst >> snd >> trd >> fth;
			if (fst >= 'a' && fst <= 'h' && trd >= 'a' && trd <= 'h' && snd >= '1' && snd <= '8' && fth >= '1' &&
				fth <= '8')
			{
				xi = fst - 'a';
				xf = trd - 'a';
				yi = snd - '1';
				yf = fth - '1';
			}
			else
			{
				message = "Invalid input";
				continue;
			}
			auto piece_at_start = game_board.find(position {xi, yi});
			if (piece_at_start.has_value())
			{
				if (piece_at_start.value().get()->color == turn)
				{
					move = piece_at_start.value().get()->move(game_board, position {xf, yf});
					if (move.state != invalid_move)
					{
						//Moved successfully
						for (auto& p: game_board.val)
						{
							if (p != nullptr)
							{
								if (p->type == e_pawn && p->color == turn && p != piece_at_start.value().get())
								{
									p->just_moved_2square = false;
								}
							}
						}
						if (game_board.is_out_of_moves(INVERT(turn)))
						{
							if (move.state == check)
							{
								DISPLAY_END_GAME(type_to_char, game_board,
										std::string("Checkmate, ").append(turn == e_white ? "white" : "black") +
										" won");
								return 0;
							}
							DISPLAY_END_GAME(type_to_char, game_board, "Stalemate, draw");
							return 0;
						}
						//Check for 3-fold repetition
						int count = 0;
						for (const auto& a: game_history)
						{
							if (a.second != turn)
							{
								continue;
							}
							auto result = a.first.compare(game_board);
							if (result == 0)
							{
								count++;
							}
							if (result == -1)
							{
								//game_history.clear();
								//break;
							}
						}
						if (count >= 2)
						{
							DISPLAY_END_GAME(type_to_char, game_board, "Three-fold repetition, draw");
							return 0;
						}
						game_history.emplace_back(game_board.clone(), turn);

						//50 moves
						if (piece_at_start.value().get()->type == e_pawn || move.state == move_state::capture || move.state == move_state::check_capture)
						{
							move_count = 0;
						}
						else
						{
							move_count += 0.5;
							if (move_count >= 49.99)
							{
								DISPLAY_END_GAME(type_to_char, game_board, "50 moves without progress, draw");
							}
						}
						turn = INVERT(turn);
					}
					else
					{
						message = "Invalid move: " + move.message;
					}
				}
				else
				{
					message = std::string("Invalid move: It's not ").append(turn == e_white ? "black's" : "white's") +
							  " turn yet";
				}
			}
			else
			{
				message = "Invalid move: No piece on " + std::string(1, xi + 'a') + // NOLINT(bugprone-narrowing-conversions)
						  std::to_string(
								  yi + 1);
			}
		}
	}
}

#pragma clang diagnostic pop

void DISPLAY_BOARD(std::map<type_enum, char>& type_to_char, board& game_board)
{
	std::array<std::array<std::pair<char, color_enum>, 8>, 8> pos_board {};
	for (auto& a: game_board.val)
	{
		if (a.get() != nullptr)
		{
			pos_board[7 - a->pos.y][a->pos.x] = std::make_pair(type_to_char[a->type], a->color);
		}
	}
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	//Also print 1->8 vertically and A->H horizontally
	int i = 8;
	for (int ind = 0; ind < pos_board.size(); ind++)
	{
		std::cout << i << " ";
		i--;
		for (int jnd = 0; jnd < pos_board[0].size(); jnd++)
		{
			int c_tile = (ind % 2 != jnd % 2 ? g_c_black_tile : g_c_white_tile);
			auto b = pos_board[ind][jnd];
			if (b.first == 0)
			{
				SetConsoleTextAttribute(h_console, c_tile);
				std::cout << "  ";
				SetConsoleTextAttribute(h_console, g_c_normal + g_c_normal_tile);
			}
			else
			{
				if (b.second == e_white)
				{
					SetConsoleTextAttribute(h_console, c_tile + g_c_white_piece);
				}
				else
				{
					SetConsoleTextAttribute(h_console, c_tile + g_c_black_piece);
				}
				std::cout << b.first << " ";
				SetConsoleTextAttribute(h_console, g_c_normal + g_c_normal_tile);
			}
		}
		std::cout << '\n';
	}
	std::cout << "  A B C D E F G H" << '\n';
}

void DISPLAY_END_GAME(std::map<type_enum, char>& type_to_char, board& game_board, std::string&& reason)
{
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h_console, g_c_normal + g_c_normal_tile);
	system("cls");
	DISPLAY_BOARD(type_to_char, game_board);
	std::cout << "Game ended: " << reason << '\n';
}