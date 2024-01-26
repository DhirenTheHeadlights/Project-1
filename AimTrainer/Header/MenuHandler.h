#pragma once

#include <functional>

#include "GameStateManager.h"
#include "StartMenu.h"
#include "OptionsMenu.h"
#include "LeaderBoardMenu.h"
#include "EndMenu.h"
#include "InsertNameMenu.h"

// This class just holds the menus and handles switching between them
class MenuHandler {
public:
	MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager* GSM);
	void setUpMenus();

	// Function to draw the menus
	void drawStartMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawOptionsMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawLeaderBoardMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawInsertNameMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawEndMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);

	// getters
	StartMenu* getStartMenu() { return startMenu.get(); }
	OptionsMenu* getOptionsMenu() { return optionsMenu.get(); }
	LeaderBoardMenu* getLeaderBoardMenu() { return leaderBoardMenu.get(); }
	EndMenu* getEndMenu() { return endMenu.get(); }

private:
	// Pointers to the menus
	std::unique_ptr<StartMenu> startMenu;
	std::unique_ptr<OptionsMenu> optionsMenu;
	std::unique_ptr<LeaderBoardMenu> leaderBoardMenu;
	std::unique_ptr<EndMenu> endMenu;
	std::unique_ptr<InsertNameMenu> insertNameMenu;
};
