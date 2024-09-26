#include "MenuHandler_PG.h"
#include "GameState_PG.h"

using namespace PirateGame;

void MenuHandler::createMenus(WindHandler& WH) {
	// Add the menus to the map
	startMenu = std::make_shared<StartMenu>(context);
	optionsMenu = std::make_shared<OptionsMenu>(context);
	HUD = std::make_shared<InGameHUD>(context, WH);
	inventoryMenu = std::make_shared<InventoryMenu>(context);

	menus.insert(std::make_pair(MenuType::StartMenu, startMenu));
	menus.insert(std::make_pair(MenuType::OptionsMenu, optionsMenu));
	menus.insert(std::make_pair(MenuType::HUD, HUD));
	menus.insert(std::make_pair(MenuType::InventoryMenu, inventoryMenu));
}

void MenuHandler::openMenu(MenuType menuType) const {
	// This will reset the view so that the menu is correctly visible. However, it should not happen w the hud.
	if (GameState::currGameState != GameState::State::GameLoop) {
		Globals::window->setView(Globals::window->getDefaultView());
	}

	for (auto& menu : menus) {
		if (menu.first == menuType) {
			menu.second->update();
			menu.second->draw();
		}
	}
}

void MenuHandler::setUpMenus() const {
	// Set up the menus
	for (auto& menu : menus) {
		menu.second->setUpMenu();
	}
}
