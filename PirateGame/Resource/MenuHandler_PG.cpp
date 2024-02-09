#include "MenuHandler_PG.h"

using namespace PirateGame;

MenuHandler::MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager& GSM) : window(window), GSM(GSM) {
	// Initialize the menu pointers and set up the menus
	startMenu = std::make_unique<StartMenu>(window, font, GSM);
	optionsMenu = std::make_unique<OptionsMenu>(window, font, GSM);
	HUD = std::make_unique<InGameHUD>(window, font, GSM);
}

void MenuHandler::openMenu(MenuType menuType) {
	switch (menuType) {
	case MenuType::StartMenu:
		startMenu->draw(window);
		break;
	case MenuType::OptionsMenu:
		optionsMenu->draw(window);
		break;
	case MenuType::HUD:
		HUD->draw(window);
		break;
	}

}

void MenuHandler::setUpMenus() {
	// Set up the menus
	startMenu->setUpMenu();
	optionsMenu->setUpMenu();
}
