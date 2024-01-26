#pragma once
#include "Menu_AT.h"

/// <summary>
/// This class is for the insert name menu, it is used to get the player's name
/// and to store it in the save data. This name is also displayed on the leader
/// board.
/// </summary>

class InsertNameMenu : public Menu {
public:
	InsertNameMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM);
	void setUpMenu();
	void drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
private:
	// Functions for the menu interactions
	void setName();

	// Lambda functions for menu interactions
	std::function<void(float value)> setNameFunc;

	// Variable to store the game state manager
	GameStateManager* GSM;

	// Colors for the menu
	sf::Color backgroundColor = sf::Color(245, 245, 245);
	sf::Color buttonColor = sf::Color(0, 191, 255);
	sf::Color sliderTrackColor = sf::Color(112, 128, 144);
	sf::Color sliderThumbColor = sf::Color(255, 99, 71);
	sf::Color interactableHoverColor = sf::Color(135, 206, 250);
	sf::Color textColor = sf::Color(34, 34, 34);
};