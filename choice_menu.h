//
// Created by Admin on 04/05/2023.
//

#ifndef CHESS_CHOICE_MENU_H
#define CHESS_CHOICE_MENU_H

#include <functional>
#include "menu.h"

class choice_menu : public menu
{
public:
	std::function<void()> action = []() -> void
	{};

	choice_menu() = default;

	choice_menu(choice_menu&&) noexcept = default;

	choice_menu(std::string_view i_text, auto action) : action(std::move(action)) {
		text = i_text;
	};

	choice_menu& operator=(choice_menu&&) noexcept = default;

	virtual ~choice_menu() = default;

	void be_selected(const int& C_Normal, const int& C_Normal_Tile) const override
	{
		action();
	}
};

#endif //CHESS_CHOICE_MENU_H