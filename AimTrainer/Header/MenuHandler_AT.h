#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

#include "GameStateManager_AT.h"
#include "StartMenu_AT.h"
#include "OptionsMenu_AT.h"
#include "LeaderBoardMenu_AT.h"
#include "EndMenu_AT.h"
#include "InsertNameMenu_AT.h"

// This class just holds the menus and handles switching between them
class MenuHandler {
public:
	MenuHandler(sf::RenderWindow& window, sf::Font& font, GameStateManager* GSM);
	void setUpMenus() const;

	// Function to draw the menus
	void drawStartMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawOptionsMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawLeaderBoardMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawInsertNameMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void drawEndMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);

	// getters
	StartMenu* getStartMenu() const { return startMenu.get(); }
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
