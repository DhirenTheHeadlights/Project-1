#include "DropDown_PG.h"

using namespace PirateGame;

// Set up the drop down menu
void DropDown::setUpInteractable(sf::Vector2f size) {
	// Set the size of the drop down menu
	this->size = size;

	// Set the background rectangle
	background.setSize(size);
	background.setFillColor(sf::Color(0, 0, 0, 150));
	background.setOutlineColor(sf::Color(255, 255, 255));
	background.setOutlineThickness(2);
	background.setPosition(position);

	// Set the text
	text.setFont(font);
	text.setCharacterSize(static_cast<unsigned int>(size.y / 2));
	text.setFillColor(sf::Color::White);
	text.setPosition(position.x + 10, position.y + size.y / 4);

	// Set up the arrow
	arrowBox.setSize(sf::Vector2f(size.y, size.y));
	arrowBox.setFillColor(sf::Color(0, 0, 0, 150));
	arrowBox.setOutlineColor(sf::Color(255, 255, 255));
	arrowBox.setOutlineThickness(2);
	arrowBox.setPosition(position.x + size.x - size.y, position.y);

	arrow.setPointCount(3);
	arrow.setPoint(0, sf::Vector2f(0, 0));
	arrow.setPoint(1, sf::Vector2f(size.y, 0));
	arrow.setPoint(2, sf::Vector2f(size.y / 2, size.y / 2));
	arrow.setFillColor(sf::Color::White);
	arrow.setPosition(position.x + size.x - size.y, position.y);


	// Set up the drop down menu
	for (int i = 0; i < options.size(); i++) {
		// Set up the option text
		sf::Text optionText(options[i], font, static_cast<unsigned int>(size.y / 2));
		optionText.setFillColor(sf::Color::White);
		optionText.setPosition(position.x, position.y + size.y + (size.y * i));

		// Set up the option box
		sf::RectangleShape optionBox(sf::Vector2f(size.x, size.y));
		optionBox.setFillColor(sf::Color(0, 0, 0, 150));
		optionBox.setOutlineColor(sf::Color(255, 255, 255));
		optionBox.setOutlineThickness(2);
		optionBox.setPosition(position.x, position.y + size.y + (size.y * i));

		// Add the option text and box to the vectors
		optionTexts.push_back(optionText);
		optionBoxes.push_back(optionBox);
	}
}

// Set the position of the drop down menu
void DropDown::setPosition(sf::Vector2f pos) {
	position = pos;
	background.setPosition(position);
	text.setPosition(position.x + 10, position.y + size.y / 4);
	arrowBox.setPosition(position.x + size.x - size.y, position.y);
	arrow.setPosition(position.x + size.x - size.y, position.y);
}

// Interact with the drop down menu
void DropDown::interact(sf::RenderWindow& window) {
	// If the mouse is over the drop down menu
	if (background.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		// If the mouse is over the arrow box
		if (arrowBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
			// If the drop down menu is not open
			if (!isOpen) {
				// Open the drop down menu
				isOpen = true;
			}
			else {
				// Close the drop down menu
				isOpen = false;
			}
		}
	}
}

// Draw the drop down menu
void DropDown::draw(sf::RenderWindow& window) {
	// Draw the background
	window.draw(background);
	window.draw(text);
	window.draw(arrowBox);
	window.draw(arrow);

	// If the drop down menu is open, open the drop down menu
	if (isOpen) {
		openDropDown(window);
	}
}

// Open the drop down menu if needed
void DropDown::openDropDown(sf::RenderWindow& window) {
	// Draw the options
	for (int i = 0; i < options.size(); i++) {
		window.draw(optionBoxes[i]);
		window.draw(optionTexts[i]);
	}

	// If the mouse is over an option
	for (int i = 0; i < options.size(); i++) {
		if (optionBoxes[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
			// If the mouse is clicked
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				// Set the selected option
				selectedOption = options[i];
				text.setString(selectedOption);
				isOpen = false;
			}
		}
	}
}