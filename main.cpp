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

int c_normal = 7, c_white_piece = 15, c_black_piece = 0, c_white_tile = 112, c_black_tile = 160, c_normal_tile = 0;

void displayBoard(std::map<type_enum, char>& type_to_char, board& game_board);

color_enum invert(color_enum _color);

//TODO: Checking for 3-fold-repetition and 50 moves (Possibly insufficient material)
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
	//Set the game_board to a simpler position for castling testing
//	board game_board {std::make_shared<king>(position {4, 0}, color_enum::e_white),
//					  std::make_shared<king>(position {4, 7}, color_enum::e_black),
//					  std::make_shared<rook>(position {0, 0}, color_enum::e_white),
//					  std::make_shared<rook>(position {7, 0}, color_enum::e_white),
//					  std::make_shared<rook>(position {0, 7}, color_enum::e_black),
//					  std::make_shared<rook>(position {7, 7}, color_enum::e_black),
//					  std::make_shared<queen>(position {5, 3}, color_enum::e_black)};
	color_enum turn = e_white;
	while (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
		move_result move {invalid_move};
		std::string input {}, word, message;
		while (move.state == invalid_move)
		{
			//Display the pieces on the board
			system("cls");
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, c_normal);
			if (!message.empty())
			{
				std::cout << message << '\n';
				message.clear();
			}
			displayBoard(type_to_char, game_board);
			int xi, yi, xf, yf;
			std::cout << "Enter a move(xi + yi + xf + yf/exit to stop program/config to change color): ";
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
					SetConsoleTextAttribute(hConsole, i);
					std::cout << i << ": " << "Hello World! ";
					SetConsoleTextAttribute(hConsole, i + 240);
					std::cout << i << ": " << "Hello World!";
					SetConsoleTextAttribute(hConsole, 0);
					std::cout << '\n';
				}
				SetConsoleTextAttribute(hConsole, c_normal + c_normal_tile);
				std::cout
						<< "Input your preferred front color (for colors of text, "
						   "white piece, black piece in that order): ";
				std::string input2;
				getline(std::cin, input2);
				std::stringstream ss2 {input2};
				ss2 >> c_normal >> c_white_piece >> c_black_piece;
				system("cls");
				//c_normal
				//Input background color
				for (int i = 0; i < 16; i++)
				{
					SetConsoleTextAttribute(hConsole, i * 16);
					std::cout << i << ": " << "Hello World! ";
					SetConsoleTextAttribute(hConsole, i * 16 + 15);
					std::cout << i << ": " << "Hello World!";
					SetConsoleTextAttribute(hConsole, 0);
					std::cout << '\n';
				}
				SetConsoleTextAttribute(hConsole, c_normal + c_normal_tile);
				std::cout
						<< "Input your preferred background color (for background colors of text, "
						   "white piece, black piece in that order): ";
				getline(std::cin, input2);
				ss2.clear();
				ss2 = std::stringstream {input2};
				ss2 >> c_normal_tile >> c_white_tile >> c_black_tile;
				c_normal_tile *= 16;
				c_white_tile *= 16;
				c_black_tile *= 16;
				system("cls");
				continue;
			}
			//If the first and 3rd word are character, convert them to numbers
			std::stringstream ss {input};
			char fst, snd, trd, fth;
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
					if (move.state == promotion || move.state == check_promotion)
					{
						std::cout << "Enter the piece you want to promote to (q, r, b, n): ";
						std::string input2;
						getline(std::cin, input2);
						if (input2 == "q")
						{
							piece_at_start.value().get().reset(new queen {position {xf, yf}, turn});
						}
						else if (input2 == "r")
						{
							piece_at_start.value().get().reset(new rook {position {xf, yf}, turn});
						}
						else if (input2 == "b")
						{
							piece_at_start.value().get().reset(new bishop {position {xf, yf}, turn});
						}
						else if (input2 == "n")
						{
							piece_at_start.value().get().reset(new knight {position {xf, yf}, turn});
						}
						else
						{
							message = "Invalid input";
							continue;
						}
					}
					if (move.state != invalid_move)
					{
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
						if (game_board.is_out_of_moves(invert(turn)))
						{
							if (move.state == check)
							{
								std::cout << std::string("Checkmate, ").append(turn == e_white ? "white" : "black") +
											 " won" << '\n';
								return 0;
							}
							else
							{
								std::cout << "Stalemate, draw" << '\n';
								return 0;
							}
						}
						turn = invert(turn);
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
				message = "Invalid move: No piece on " + std::string(1, xi + 'a') + std::to_string(yi + 1);
			}
		}
	}
}

void displayBoard(std::map<type_enum, char>& type_to_char, board& game_board)
{
	std::array<std::array<std::pair<char, color_enum>, 8>, 8> pos_board {};
	for (auto& a: game_board.val)
	{
		if (a.get() != nullptr)
		{
			pos_board[7 - a->pos.y][a->pos.x] = std::make_pair(type_to_char[a->type], a->color);
		}
	}
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//Also print 1->8 vertically and A->H horizontally
	int i = 8;
	for (int ind = 0; ind < pos_board.size(); ind++)
	{
		std::cout << i << " ";
		i--;
		for (int jnd = 0; jnd < pos_board[0].size(); jnd++)
		{
			int c_tile = (ind % 2 != jnd % 2 ? c_black_tile : c_white_tile);
			auto b = pos_board[ind][jnd];
			if (b.first == 0)
			{
				SetConsoleTextAttribute(hConsole, c_tile);
				std::cout << "  ";
				SetConsoleTextAttribute(hConsole, c_normal + c_normal_tile);
			}
			else
			{
				if (b.second == e_white)
				{
					SetConsoleTextAttribute(hConsole, c_tile + c_white_piece);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, c_tile + c_black_piece);
				}
				std::cout << b.first << " ";
				SetConsoleTextAttribute(hConsole, c_normal + c_normal_tile);
			}
		}
		std::cout << '\n';
	}
	std::cout << "  A B C D E F G H" << '\n';
}