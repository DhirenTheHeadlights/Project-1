#include "MenuHandler_PG.h"

using namespace PirateGame;

void MenuHandler::createMenus() {
	// Add the menus to the map
	startMenu = std::make_shared<StartMenu>(context);
	optionsMenu = std::make_shared<OptionsMenu>(context);
	HUD = std::make_shared<InGameHUD>(context);
	inventoryMenu = std::make_shared<InventoryMenu>(context);

	menus.insert(std::make_pair(MenuType::StartMenu, startMenu));
	menus.insert(std::make_pair(MenuType::OptionsMenu, optionsMenu));
	menus.insert(std::make_pair(MenuType::HUD, HUD));
	menus.insert(std::make_pair(MenuType::InventoryMenu, inventoryMenu));
}

void MenuHandler::openMenu(MenuType menuType) const {
	// This will reset the view so that the menu is correctly visible. However, it should not happen w the hud.
	if (context.GGSM->getCurrentGameState() != GameState::GameLoop) {
		context.GV->getWindow()->setView(context.GV->getWindow()->getDefaultView());
	}

	for (auto& menu : menus) {
		if (menu.first == menuType) {
			menu.second->update();
			menu.second->draw();
		}
	}
}

void MenuHandler::setUpMenus() {
	// Set up the menus
	for (auto& menu : menus) {
		menu.second->setUpMenu();
	}
}