#include "MenuHandler_PG.h"

MenuHandler::MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager& GSM) : window(window), font(font), GSM(GSM) {
	// Initialize the menu pointers
	startMenu = std::make_unique<StartMenu>(window, font, GSM);
}

void MenuHandler::openMenu(MenuType menuType) {
	switch (menuType) {
	case MenuType::StartMenu:
		startMenu->setUpMenu();
		startMenu->draw(window);
		break;
	}
}
