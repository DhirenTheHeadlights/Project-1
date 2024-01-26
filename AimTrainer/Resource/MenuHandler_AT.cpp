#include "MenuHandler_AT.h"

MenuHandler::MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager* GSM) {
	// Create the menus
	startMenu = std::make_unique<StartMenu>(window, font, GSM);
	optionsMenu = std::make_unique<OptionsMenu>(window, font, GSM);
	leaderBoardMenu = std::make_unique<LeaderBoardMenu>(window, font, GSM);
	endMenu = std::make_unique<EndMenu>(window, font, GSM);
	insertNameMenu = std::make_unique<InsertNameMenu>(window, font, GSM);
}

void MenuHandler::setUpMenus() {
	// Set up the menus
	startMenu->setUpMenu();
	optionsMenu->setUpMenu();
	leaderBoardMenu->setUpMenu();
	endMenu->setUpMenu();
	insertNameMenu->setUpMenu();
}

void MenuHandler::drawStartMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
	// Draw the start menu
	startMenu->drawMenu(currentEvent, mainClock, crosshair);
}

void MenuHandler::drawOptionsMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
	// Draw the options menu
	optionsMenu->drawMenu(currentEvent, mainClock, crosshair);
}

void MenuHandler::drawLeaderBoardMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
	// Draw the leader board menu
	leaderBoardMenu->drawMenu(currentEvent, mainClock, crosshair);
}

void MenuHandler::drawInsertNameMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
	// Draw the insert name menu
	insertNameMenu->drawMenu(currentEvent, mainClock, crosshair);
}

void MenuHandler::drawEndMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
	// Draw the end menu
	endMenu->drawMenu(currentEvent, mainClock, crosshair);
}
