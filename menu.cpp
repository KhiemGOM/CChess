//
// Created by Admin on 03/05/2023.
//

#include "menu.h"
#include <iostream>
#include <Windows.h>
#include <sstream>

//#include "main.cpp"
void menu::display(const int& C_Normal, const int& C_Normal_Tile) const
{
	const auto NUM_OF_OPTIONS = inner_menu.size();
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
	std::string message {};
	while (true)
	{
		system("cls");
		std::cout << text << ":" << std::endl;
		std::cout <<"0. Back" << std::endl;
		for (int i = 0; i < NUM_OF_OPTIONS; ++i)
		{
			std::cout << i + 1 << ". " << inner_menu[i]->text << std::endl;
		}
		if (!message.empty())
		{
			std::cout << message << std::endl;
			message.clear();
		}
		std::cout << "Please enter the index of the menu you want to navigate to: ";
		int index = 0;
		std::string input;
		getline(std::cin, input);
		std::stringstream ss{input};
		ss >> index;
		--index;
		if (index == - 1)
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

void menu::be_selected(const int& C_Normal, const int& C_Normal_Tile) const
{
	display(C_Normal, C_Normal_Tile);
}