#pragma once
#include "Menu.h"

// This class is for the options menu

class OptionsMenu : public Menu {
public:
	OptionsMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM);
	void setUpMenu();
	void drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair);
private:
	// Functions for the menu interactions
	void maxTarget();
	void setTargetSize(float newSize);
	void setMouseSensitivity(float sens);

	// Lambda functions for menu interactions
	std::function<void(float value)> maxTargetFunc;
	std::function<void(float value)> setTargetSizeFunc;
	std::function<void(float value)> setMouseSensitivityFunc;

	// Variable to store the current game state
	GameStateManager* GSM;

	// Colors for the menu
	sf::Color backgroundColor = sf::Color(245, 245, 245);
	sf::Color buttonColor = sf::Color(0, 191, 255);
	sf::Color sliderTrackColor = sf::Color(112, 128, 144);
	sf::Color sliderThumbColor = sf::Color(255, 99, 71);
	sf::Color interactableHoverColor = sf::Color(135, 206, 250);
	sf::Color textColor = sf::Color(34, 34, 34);

	// Variable to store whether the tab key was pressed last frame
	bool wasTabPressedLastFrame = false;
};
