//
// Created by Admin on 04/05/2023.
//

#ifndef CHESS_MULTIPLE_CHOICE_MENU_H
#define CHESS_MULTIPLE_CHOICE_MENU_H

#include "menu.h"
#include <functional>
#include <windows.h>
#include <iostream>

template<class Arg>
requires
requires (Arg&& arg){
	std::cout << arg;
}
class multiple_choice_menu : public menu
{
public:
	std::function<Arg()> input_supplier {};

	multiple_choice_menu() = default;

	multiple_choice_menu(multiple_choice_menu&&) noexcept = default;

	multiple_choice_menu& operator=(multiple_choice_menu&&) noexcept = default;

	virtual ~multiple_choice_menu() = default;

	multiple_choice_menu(std::string_view text, std::initializer_list<sharedPtr> list, auto input)
		:
		input_supplier(std::move(input))
	{
		int i = 0;
		inner_menu = {list.size(), sharedPtr {}};
		for (const sharedPtr& child: list)
		{
			inner_menu[i] = child;
			++i;
		}
		this->text = text;
	};

	void be_selected(const int& C_Normal, const int& C_Normal_Tile) const override
	{
		const auto NUM_OF_OPTIONS = inner_menu.size();
		HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
		std::string message {};
		while (true)
		{
			system("cls");
			std::cout << text << ":" << std::endl;
			std::cout << "0. Back" << std::endl;
			for (int i = 0; i < NUM_OF_OPTIONS; ++i)
			{
				std::cout << i + 1 << ". " << inner_menu[i]->text << std::endl;
			}
			std::cout << text << " currently is: " << input_supplier() << std::endl;
			if (!message.empty())
			{
				std::cout << message << std::endl;
				message.clear();
			}
			std::cout << "Please enter the index of the option you want to choose: ";
			int index = 0;
			std::cin >> index;
			--index;
			if (index == -1)
			{
				return;
			}
			if (index < NUM_OF_OPTIONS)
			{
				inner_menu[index]->be_selected(C_Normal, C_Normal_Tile);
				continue;
			}
			message = "Invalid input, please try again";
		}
	}
};

#endif //CHESS_MULTIPLE_CHOICE_MENU_H