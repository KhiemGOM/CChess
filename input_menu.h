//
// Created by Admin on 03/05/2023.
//

#ifndef CHESS_INPUT_MENU_H
#define CHESS_INPUT_MENU_H

#include "menu.h"
#include <sstream>
#include <windows.h>

#include <utility>
#include <iostream>

template<class Arg>
requires
requires(Arg arg)
{
	std::cin >> arg;
	std::cout << arg;
} &&
std::is_default_constructible_v<Arg>
class input_menu : public menu
{
public:
	std::function<void(Arg arg)> do_with_input = [](Arg arg) -> void
	{};
	std::function<void()> ask_for_input = []() -> void
	{};
	std::function<Arg()> input_supplier = []() -> Arg
	{
		return Arg {};
	};

	virtual ~input_menu() = default;

	input_menu(std::string_view text, std::function<void()> pre_text, std::function<void(Arg arg)> action,
			   std::function<Arg()> input)
		: ask_for_input(std::move(pre_text)), do_with_input(std::move(action)), input_supplier(std::move(input))
	{
		this->text = text;
	};

	input_menu(std::string_view text, std::function<void()> pre_text, std::function<void(Arg arg)> action,
			   Arg& arg)
		: ask_for_input(std::move(pre_text)), do_with_input(std::move(action)), input_supplier([&]()
	{
			return arg;
	})
	{
		this->text = text;
	};

	input_menu() = default;

	input_menu(input_menu& other) = delete;

	input_menu(input_menu&& other) noexcept = default;

	input_menu& operator=(const input_menu& other) = delete;

	input_menu& operator=(input_menu&& other) noexcept = default;

	void be_selected(const int& C_Normal, const int& C_Normal_Tile) const override
	{
		system("cls");
		ask_for_input();
		std::cout << text << " currently is: " << input_supplier() << std::endl;
		std::cout << "Input your preferred " << text << ": ";
		HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h_console, C_Normal + C_Normal_Tile);
		Arg arg {};
		std::string input {};
		getline(std::cin, input);
		std::stringstream ss{input};
		ss >> arg;
		do_with_input(arg);
		std::cout << text << "has been changed to" << arg;
	};
};

#endif //CHESS_INPUT_MENU_H