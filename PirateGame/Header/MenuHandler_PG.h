#pragma once

/// <summary>
/// This class handles all the menu related functions for the game.
/// It also contains every menu object and their respective functions.
/// </summary>

#include "StartMenu_PG.h"

namespace PirateGame {
	// Enum for the different types of menus
	enum class MenuType {
		StartMenu,
	};

	class MenuHandler {
	public:
		MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager& GSM);
		~MenuHandler() {};

		void openMenu(MenuType menuType);
	private:
		// Pointers to the different menus
		std::unique_ptr<StartMenu> startMenu;
		//...

		// SFML objects
		sf::RenderWindow& window;
		sf::Font& font;

		// GameStateManager
		GameStateManager& GSM;
	};
}
