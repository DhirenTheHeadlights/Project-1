#pragma once

/// <summary>
/// This class handles all the menu related functions for the game.
/// It also contains every menu object and their respective functions.
/// </summary>

#include "StartMenu_PG.h"
#include "OptionsMenu_PG.h"
#include "InGameHUD_PG.h"

namespace PirateGame {
	// Enum for the different types of menus
	enum class MenuType {
		StartMenu,
		OptionsMenu,
		HUD,
	};

	class MenuHandler {
	public:
		MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager& GSM);
		~MenuHandler() {};

		void setUpMenus();
		void openMenu(MenuType menuType);
	private:
		// Pointers to the different menus
		std::unique_ptr<StartMenu> startMenu;
		std::unique_ptr<OptionsMenu> optionsMenu;
		std::unique_ptr<InGameHUD> HUD;
		//...

		// SFML objects
		sf::RenderWindow& window;

		// GameStateManager
		GameStateManager& GSM;
	};
}
