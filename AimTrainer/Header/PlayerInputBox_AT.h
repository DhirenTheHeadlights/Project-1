#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

/// <summary>
/// This class is for the player input box, it is used to get the player's name
/// and to display it on the leader board.
/// </summary>

class PlayerInputBox {
public:
	// Constructor
	PlayerInputBox();

	// Setters for the player input box
	void setFont(sf::Font& font);
	void setDefaultText(std::string text);
	void setOnlyNumbers(bool onlyNumbers);
	void setMaxCharacters(int maxCharacters);
	void setString(std::string text);

	// Main method for player input box creation and interaction
	void createPlayerInputBox(sf::RenderWindow& window, sf::Event event);

	// Draw the player input box
	void drawPlayerInputBox(sf::RenderWindow& window, sf::Event event);

	// Get the box rectangle shape
	sf::RectangleShape& getPlayerInputBox();

	// Get the box text
	sf::Text& getPlayerInputText();

	// Get the clicked variable to update other classes if needed
	bool getClicked();

	// Get the text string
	std::string getString();

	// Set if the text box should be reset when clicked out of
	void setResetTextBox(bool resetTextBox);

private:
	// Rectangle shape for the player input box
	sf::RectangleShape playerInputBox;

	// Text for the player input box
	sf::Text playerInputText;

	// Values for the player input box
	std::string text;
	std::string defaultText;
	int textSize = 30;
	int rectangleWidth = 300;
	int rectangleHeight = 50;
	int maxCharacters = 50;

	// Boolean for checking if the player input box is clicked
	bool clicked = false;

	// Boolean for setting the input box to only accept numbers
	bool onlyNumbers = false;

	// Boolean for resetting the text box when clicked out of
	bool resetTextBox = false;
};
