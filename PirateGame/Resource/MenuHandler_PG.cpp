#include "MenuHandler_PG.h"

using namespace PirateGame;

void MenuHandler::createMenus() {
	// Initialize the menu pointers and set up the menus
	startMenu = std::make_unique<StartMenu>(GSM);
	optionsMenu = std::make_unique<OptionsMenu>(GSM);
	HUD = std::make_unique<InGameHUD>(GSM);
}

void MenuHandler::openMenu(MenuType menuType) {
	switch (menuType) {
	case MenuType::StartMenu:
		startMenu->draw();
		break;
	case MenuType::OptionsMenu:
		optionsMenu->draw();
		break;
	case MenuType::HUD:
		HUD->draw();
		break;
	}
}

void MenuHandler::setUpMenus() {
	// Set up the menus
	startMenu->setUpMenu();
	optionsMenu->setUpMenu();
	HUD->setUpMenu();
}