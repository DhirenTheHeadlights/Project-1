#include "MenuHandler_PG.h"

using namespace PirateGame;

void MenuHandler::createMenus() {
	// Initialize the menu pointers and set up the menus
	startMenu = std::make_unique<StartMenu>();
	optionsMenu = std::make_unique<OptionsMenu>();
	HUD = std::make_unique<InGameHUD>();
	inventoryMenu = std::make_unique<InventoryMenu>();
}

void MenuHandler::openMenu(MenuType menuType) {
	// This will reset the view so that the menu is correctly visible. However, it should not happen w the hud.
	if (menuType != MenuType::HUD && menuType != MenuType::InventoryMenu) {
		GlobalValues::getInstance().getWindow()->setView(GlobalValues::getInstance().getWindow()->getDefaultView());
	}
	switch (menuType) {
	case MenuType::StartMenu:
		startMenu->draw();
		startMenu->update();
		break;
	case MenuType::OptionsMenu:
		optionsMenu->draw();
		optionsMenu->update();
		break;
	case MenuType::HUD:
		HUD->draw();
		HUD->update();
		break;
	case MenuType::InventoryMenu:
		inventoryMenu->draw();
		inventoryMenu->update();
		break;
	}
}

void MenuHandler::setUpMenus() {
	// Set up the menus
	startMenu->setUpMenu();
	optionsMenu->setUpMenu();
	HUD->setUpMenu();
	inventoryMenu->setUpMenu();
}