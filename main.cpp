#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <processenv.h>
#include <sstream>
#include "menu.h"
#include "multiple_choice_menu.h"
#include "pieces.h"
#include "board.h"
#include "king.h"
#include "queen.h"
#include "pawn.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "enum.h"
#include <string>
#include <wincon.h>
#include <fstream>
#include "move_result.h"
#include "input_menu.h"
#include "choice_menu.h"
#include "text_menu.h"

int C_Normal = 7, C_White_Piece = 15, C_Black_Piece = 0, C_White_Tile = 112, C_Black_Tile = 160, C_Normal_Tile = 0;

void DisplayEndGame(std::map<type_enum, char>& type_to_char, board& game_board, std::string&& reason);

void DisplayBoard(std::map<type_enum, char>& type_to_char, board& game_board);

color_enum Invert(color_enum color);

void DemoTextColor(HANDLE h_console);

void DemoBackgroundColor(HANDLE h_console);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "modernize-use-trailing-return-type"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"

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
	//Set the game_board to a simpler position for insufficient material
//	game_board = board {std::make_shared<king>(position {6,5}, e_white),
//						std::make_shared<king>(position {7,7}, e_black),
//						std::make_shared<bishop>(position {0,6}, e_white),
//						std::make_shared<bishop>(position {3,6}, e_black)};
	std::vector<std::pair<board, color_enum>> game_history {};
	color_enum turn = e_white;
	type_enum promotion_type = e_empty;
	float move_count = 0.0F;
	int full_move_count = 1;
	bool standard_notation = true;
	while (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
		move_result move {invalid_move};
		std::string input {}, word, message;
		while (move.state == invalid_move)
		{
			//Display the pieces on the board
			system("cls");
			HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(h_console, C_Normal);
			if (!message.empty())
			{
				std::cout << message << '\n';
				message.clear();
			}
			DisplayBoard(type_to_char, game_board);
			int xi {}, yi {}, xf {}, yf {};
			std::cout
				<< "Notation: " + std::string {standard_notation ? "Standard notation" : "Long Algebraic notation"} +
				   "\n"
				   "Turn: " + std::string {turn == e_white ? "White" : "Black"} + "\n"
																				  "Move count: " +
				   std::to_string(full_move_count) + "\n"
													 R"(Enter a move (type "setting" to open up settings): )";
			do
			{
				std::getline(std::cin, input);
			} while (input == "\n");
			if (input.empty())
			{
				continue;
			}
			using init_list = std::initializer_list<std::shared_ptr<menu>>;
			menu settings
				{
					"Settings",
					{
						std::make_shared<text_menu>("Help",
							"RULES:\n"
							"The game is played on an 8x8 board with two players, each\n"
							"with 16 pieces. The goal is to checkmate the opponent's\n"
							"king, which means the king is in a position to be captured\n"
							"and cannot escape capture on the next move.\n"
							"\n"
							"SPECIAL MOVES:\n"
							"- Castling: Move the king two spaces towards a rook, which\n"
							"  then moves to the other side of the king.\n"
							"- En Passant: Capture an opponent's pawn that has moved two\n"
							"  spaces forward and is next to your pawn.\n"
							"- Promotion: When a pawn reaches the opposite end of the\n"
							"  board, it can be promoted to a queen, bishop, knight, or\n"
							"  rook.\n"
							"For more information: https://en.wikipedia.org/wiki/Rules_of_chess\n"
							"\n"
							"GAME CONTROLS:\n"
							"Type the move you want to make in the appropriate notation.\n"
							"For information about move notation, please refer to the notation setting\n"
							"or: https://en.wikipedia.org/wiki/Algebraic_notation_(chess)\n"
							"\n"
							"SAVING AND LOADING GAMES:\n"
							"- Saving the game will give you a FEN string on screen and into a file.\n"
							"- The save file directory is [CChess Directory]/saves/\n"
							"- Paste the FEN string into the load menu to load the game.\n"
							"\n"
							"TROUBLESHOOTING:\n"
							"If the game is stuck or not responding, try closing and\n"
							"restarting the program. If you encounter any other issues,\n"
							"please contact me on github or open an issue for assistance.\n"
							"Please note that this game is still in development. One reoccurring\n"
							"issue is that the colors of text and clear screen relies on Windows API\n"
							"which may not work on many OSes and cause some issues with the\n"
							"render itself.\n"
							"\n"
							"CREDITS AND LINKS:\n"
							"This game was developed by KhiemGOM.\n"
							"My github page: https://github.com/KhiemGOM\n"
							"Check out the source code or report an issue here: https://github.com/KhiemGOM/CChess"
						),
						std::make_shared<menu>("Appearances", init_list
							{
								std::make_shared<input_menu<int>>(
									"Color of Black Piece",
									[&]()
									{
										DemoTextColor(h_console);
									},
									[&](int input)
									{
										C_Black_Piece = input;
									},
									C_Black_Piece
								),
								std::make_shared<input_menu<int>>(
									"Color of White Piece",
									[&]()
									{
										DemoTextColor(h_console);
									},
									[&](int input)
									{
										C_White_Piece = input;
									},
									C_White_Piece
								),
								std::make_shared<input_menu<int>>(
									"Color of Text",
									[&]()
									{
										DemoTextColor(h_console);
									},
									[&](int input)
									{
										C_Normal = input;
									},
									C_Normal
								),
								std::make_shared<input_menu<int>>(
									"Color of Black Tile",
									[&]()
									{
										DemoBackgroundColor(h_console);
									},
									[&](int input)
									{
										C_Black_Tile = input * 16;
									},
									[&]()
									{
										return C_Black_Tile / 16;
									}
								),
								std::make_shared<input_menu<int>>(
									"Color of White Tile",
									[&]()
									{
										DemoBackgroundColor(h_console);
									},
									[&](int input)
									{
										C_White_Tile = input * 16;
									},
									[&]()
									{
										return C_White_Tile / 16;
									}
								),
								std::make_shared<input_menu<int>>(
									"Color of Text Background",
									[&]()
									{
										DemoBackgroundColor(h_console);
									},
									[&](int input)
									{
										C_Normal_Tile = input * 16;
									},
									[&]()
									{
										return C_Normal_Tile / 16;
									}
								)
							}),
						std::make_shared<multiple_choice_menu<std::string_view>>(
							"Notation",
							init_list
								{
									std::make_shared<choice_menu>(
										"Standard Notation (Nxe4/e8=Q)",
										[&]()
										{
											standard_notation = true;
										}
									),
									std::make_shared<choice_menu>(
										"Long Algebraic Notation (e2e4, no x for capture)",
										[&]()
										{
											standard_notation = false;
										}
									)
								},
							[&]()
							{
								return standard_notation ? "Standard Notation" : "Long Algebraic Notation";
							}
						),
						std::make_shared<menu>("Save and Load", init_list
							{
								std::make_shared<text_menu>("Save",
									[&]()
									{
										return "This is the FEN string of the current position:\n" +
											   (std::string)game_board.save(turn, (int)std::round(move_count * 2),
												   full_move_count) +
											   "\nYour current position is also saved into the saves folder";
									},
									[&]()
									{
										auto current_time = std::chrono::system_clock::now();
										auto current_time_t = std::chrono::system_clock::to_time_t(current_time);
										std::stringstream ss {};
										ss << std::put_time(std::localtime(&current_time_t), "%d-%m-%Y_%H-%M-%S");
										std::ofstream save_file {"./saves/" + ss.str() + ".fen"};
										save_file
											<< game_board.save(turn, (int)std::round(move_count * 2), full_move_count);
									}
								),
								std::make_shared<text_menu>("Load",
									[&]()
									{
										return "Make sure to save your progress and prevent losing your current position\n"
											   "Please enter the FEN string of the position you want to load:";
									},
									[&]()
									{
										board temp {};
										std::string fen {};
										std::getline(std::cin, fen);
										int temp_move_count = 0, temp_full_move = 0;
										color_enum temp_turn = e_white;
										auto err = temp.load(fen, temp_move_count, temp_full_move, temp_turn);
										if (err.empty())
										{
											game_board = temp;
											move_count = (float)temp_move_count / 2;
											full_move_count = temp_full_move;
											turn = temp_turn;
											std::cout << "Successfully loaded\n";
											return;
										}
										std::cout << "Invalid FEN string: " << err << "\n";
									}
								)
							}),
						std::make_shared<choice_menu>(
							"Exit",
							[&]()
							{
								std::exit(0);
							}
						)
					}
				};
			if (input == "exit")
			{
				std::exit(0);
			}
			if (input == "setting")
			{
				settings.display(C_Normal, C_Normal_Tile);
				continue;
			}
			std::stringstream ss {input};
			std::optional<std::reference_wrapper<std::shared_ptr<pieces>>> piece_at_start {};
			if (!standard_notation)
			{
				//If the first and 3rd word are character, convert them to numbers
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
			}
			else
			{
				auto move_r = game_board.interpret(ss.str(), turn);
				if (move_r.state != invalid_move)
				{
					xi = move_r.from.x;
					yi = move_r.from.y;
					xf = move_r.to.x;
					yf = move_r.to.y;
					promotion_type = move_r.promotion_type;
				}
				else
				{
					message = "Invalid input: " + move_r.err;
					continue;
				}
			}
			piece_at_start = game_board.find(position {xi, yi});
			if (!piece_at_start.has_value())
			{
				message = "Invalid move: No piece on " + std::string(1, xi + 'a') + std::to_string(yi + 1); //NOLINT
				continue;
			}

			if (piece_at_start.value().get()->color != turn)
			{
				message = std::string("Invalid move: It's not ").append(turn == e_white ? "black's" : "white's") +
						  " turn yet";
				continue;
			}

			move = piece_at_start.value().get()->move(game_board, position {xf, yf}, promotion_type);
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
				if (game_board.is_out_of_moves(Invert(turn)))
				{
					if (move.state == check)
					{
						DisplayEndGame(type_to_char, game_board,
							std::string("Checkmate, ").append(turn == e_white ? "white" : "black") +
							" won");
					}
					DisplayEndGame(type_to_char, game_board, "Stalemate, draw");
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
					DisplayEndGame(type_to_char, game_board, "Three-fold repetition, draw");
				}
				game_history.emplace_back(game_board.clone(), turn);

				//50 moves
				if (piece_at_start.value().get()->type == e_pawn || move.state == move_state::capture ||
					move.state == move_state::check_capture)
				{
					move_count = 0;
				}
				else
				{
					move_count += 0.5;
					if (move_count >= 49.99)
					{
						DisplayEndGame(type_to_char, game_board, "50 moves without progress, draw");
					}
				}

				if (turn == e_black)
				{
					++full_move_count;
				}

				//Insufficient material
				if (game_board.is_insufficient_material())
				{
					DisplayEndGame(type_to_char, game_board, "Insufficient material, draw");
				}
				turn = Invert(turn);
			}
			else
			{
				message = "Invalid move: " + move.message;
			}
		}
	}
}

void DemoBackgroundColor(HANDLE h_console)
{
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
	SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
}

void DemoTextColor(HANDLE h_console)
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
	SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
}

#pragma clang diagnostic pop

void DisplayBoard(std::map<type_enum, char>& type_to_char, board& game_board)
{
	std::array<std::array<std::pair<char, color_enum>, 8>, 8> pos_board {};
	for (const auto& a: game_board.val)
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
			int c_tile = (ind % 2 != jnd % 2 ? C_Black_Tile : C_White_Tile);
			auto b = pos_board[ind][jnd];
			if (b.first == 0)
			{
				SetConsoleTextAttribute(h_console, c_tile);
				std::cout << "  ";
				SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
			}
			else
			{
				if (b.second == e_white)
				{
					SetConsoleTextAttribute(h_console, c_tile + C_White_Piece);
				}
				else
				{
					SetConsoleTextAttribute(h_console, c_tile + C_Black_Piece);
				}
				std::cout << b.first << " ";
				SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
			}
		}
		std::cout << '\n';
	}
	std::cout << "  A B C D E F G H" << '\n';
}

void DisplayEndGame(std::map<type_enum, char>& type_to_char, board& game_board, std::string&& reason)
{
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
	system("cls");
	DisplayBoard(type_to_char, game_board);
	std::cout << "Game ended: " << reason << '\n';
	std::exit(0);
}