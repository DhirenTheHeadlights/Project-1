#pragma once

/// <summary>
/// This class handles all the menu related functions for the game.
/// It also contains every menu object and their respective functions.
/// </summary>

#include "Menu_PG.h"

// Enum for the different types of menus
enum class MenuType {
	StartMenu,
};

class MenuHandler {
public:
	MenuHandler(sf::RenderWindow& window, sf::Font& font);
	~MenuHandler();

	void openMenu(MenuHandler::MenuType& menuType);
private:
	// Pointers to the different menus
	std::unique_ptr<StartMenu> startMenu;
};

