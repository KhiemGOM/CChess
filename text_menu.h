//
// Created by Admin on 04/05/2023.
//

#ifndef CHESS_TEXT_MENU_H
#define CHESS_TEXT_MENU_H

#include "menu.h"
#include <iostream>
#include <conio.h>

class text_menu : public menu
{
public:
	std::function<void()> action = []()
	{};
	std::function<std::string()> info_supplier = []()
	{ return ""; };
	std::string info;

	text_menu() = default;

	text_menu(std::string_view text, std::string_view info) : info(info)
	{
		this->text = text;
	};

	text_menu(std::string_view text, auto info_sup, auto action) : info_supplier(std::move(info_sup)), action(std::move(action))
	{
		this->text = text;
	};

	text_menu(const text_menu&) = default;

	text_menu(text_menu&&) noexcept = default;

	text_menu& operator=(const text_menu&) = default;

	text_menu& operator=(text_menu&&) noexcept = default;

	virtual ~text_menu() = default;

	void be_selected(const int& C_Normal, const int& C_Normal_Tile) const override
	{
		system("cls");
		HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
		std::string temp = info_supplier();
		if (temp.empty())
		{
			temp = info;
		}
		std::cout << temp << std::endl;
		action();
		std::cout << "Press any button to continue";
		while (!_kbhit())
		{}
		_getch();
	}
};

#endif //CHESS_TEXT_MENU_H