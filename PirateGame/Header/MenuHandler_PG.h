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
		void openMenu(MenuType menuType) const;

		StartMenu* getStartMenu() const { return startMenu.get(); }
		OptionsMenu* getOptionsMenu() { return optionsMenu.get(); }
		InGameHUD* getHUD() { return HUD.get(); }
		InventoryMenu* getInventoryMenu() { return inventoryMenu.get(); }
		
	private:
		// Reference to the global context
		GlobalContext& context;

		// Map of menu types and game states to the menu objects
		std::unordered_map<MenuType, std::shared_ptr<Menu>> menus;

		// Pointers to the different menus
		std::shared_ptr<StartMenu> startMenu;
		std::shared_ptr<OptionsMenu> optionsMenu;
		std::shared_ptr<InGameHUD> HUD;
		std::shared_ptr<InventoryMenu> inventoryMenu;
		//...
	};
}
