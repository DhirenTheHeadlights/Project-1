#include "MenuHandler_PG.h"

using namespace PirateGame;

MenuHandler::MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager& GSM) : window(window), font(font), GSM(GSM) {
	// Initialize the menu pointers and set up the menus
	startMenu = std::make_unique<StartMenu>(window, font, GSM);
	startMenu->setUpMenu();
}

void MenuHandler::openMenu(MenuType menuType) {
	switch (menuType) {
	case MenuType::StartMenu:
		startMenu->draw(window);
		break;
	}
}
