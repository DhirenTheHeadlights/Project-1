#pragma once

/// <summary>
/// This class handles all the menu related functions for the game.
/// It also contains every menu object and their respective functions.
/// </summary>

#include "StartMenu_PG.h"
#include "OptionsMenu_PG.h"
#include "InGameHUD_PG.h"
#include "InventoryMenu_PG.h"

namespace PirateGame {
	// Enum for the different types of menus
	enum class MenuType {
		StartMenu,
		OptionsMenu,
		HUD,
		InventoryMenu
	};

	class MenuHandler {
	public:
		MenuHandler(GlobalContext& context) : context(context) {};
		~MenuHandler() {};

		void createMenus();
		void setUpMenus();
		void openMenu(MenuType menuType);

		StartMenu* getStartMenu() { return startMenu.get(); }
		OptionsMenu* getOptionsMenu() { return optionsMenu.get(); }
		InGameHUD* getHUD() { return HUD.get(); }
		InventoryMenu* getInventoryMenu() { return inventoryMenu.get(); }
		
	private:
		// Reference to the global context
		GlobalContext& context;

		// Pointers to the different menus
		std::unique_ptr<StartMenu> startMenu;
		std::unique_ptr<OptionsMenu> optionsMenu;
		std::unique_ptr<InGameHUD> HUD;
		std::unique_ptr<InventoryMenu> inventoryMenu;
		//...
	};
}
