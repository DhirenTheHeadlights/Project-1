#pragma once
#include "Menu_AT.h"
#include "SaveData_AT.h"

// This class is for the leader board menu

class LeaderBoardMenu : public Menu {
public:
	LeaderBoardMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM);
	void setUpMenu();
	void drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
	void setUpLeaderBoard();
private:
	// Functions for the menu interactions
	void returnToTitleScreen() const;
	// No function for showing the leaderboard

	// Lambda functions for menu interactions
	std::function<void(float value)> returnToTitleFunc;
	// No lambda function for showing the leaderboard

	// Variable to store the game state manager
	GameStateManager* GSM;

	// Variable to store the save data
	SaveData leaderboardData;

	// Colors for the menu
	sf::Color backgroundColor = sf::Color(245, 245, 245);
	sf::Color buttonColor = sf::Color(0, 191, 255);
	sf::Color sliderTrackColor = sf::Color(112, 128, 144);
	sf::Color sliderThumbColor = sf::Color(255, 99, 71);
	sf::Color interactableHoverColor = sf::Color(135, 206, 250);
	sf::Color textColor = sf::Color(34, 34, 34);
};