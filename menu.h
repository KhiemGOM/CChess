//
// Created by Admin on 03/05/2023.
//

#ifndef CHESS_MENU_H
#define CHESS_MENU_H

#include <array>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>

class menu
{
protected:
	using sharedPtr = std::shared_ptr<menu>;
public:
	std::string text;
	std::vector<sharedPtr> inner_menu {};

	menu() = default;

	menu(const menu&) = default;

	menu(menu&&) noexcept = default;

	menu& operator=(const menu&) = default;

	menu& operator=(menu&&) noexcept = default;

	~menu() = default;

	menu(std::initializer_list<sharedPtr> list)
	{
		int i = 0;
		inner_menu = {list.size(), sharedPtr {}};
		for (const sharedPtr& child: list)
		{
			inner_menu[i] = child;
			++i;
		}
	};

	menu(std::string_view text, std::initializer_list<sharedPtr> list) : text(text)
	{
		int i = 0;
		inner_menu = {list.size(), sharedPtr {}};
		for (const sharedPtr& child: list)
		{
			inner_menu[i] = child;
			++i;
		}
	};

	explicit menu(std::string_view text) : text(text)
	{};

	void display(const int& C_Normal, const int& C_Normal_Tile) const;

	virtual void be_selected(const int& C_Normal, const int& C_Normal_Tile) const;
};

#endif //CHESS_MENU_H