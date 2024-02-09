#include "DropDown_PG.h"

using namespace PirateGame;

// Set up the drop down menu
void DropDown::setUpInteractable(sf::Vector2f size) {
	// Set the size of the drop down menu
	this->size = size;

	// Set the rectangle shapes
	background.setSize(size);
	foreground.setSize(sf::Vector2f(size.x - padding * 2, size.y - padding * 2));
	frame.setSize(size);

	// Set the text
	text.setFont(font);
	text.setCharacterSize(static_cast<unsigned int>(size.y / 2));
	text.setPosition(position.x + 10, position.y + size.y / 4);

	// Set the selected text
	selectedText.setFont(font);
	selectedText.setCharacterSize(static_cast<unsigned int>(size.y / 2));

	// Set up the drop down menu
	for (int i = 0; i < options.size(); i++) {
		// Set up the option text
		sf::Text optionText(options[i].second, font, static_cast<unsigned int>(size.y / 3));
		optionText.setFillColor(sf::Color::White);

		// Set up the option box
		sf::RectangleShape optionBox(sf::Vector2f(size.x - padding * 2, size.y - padding * 2));
		optionBox.setFillColor(sf::Color(0, 0, 0, 150));

		// Add the option text and box to the vectors
		optionTexts.push_back(optionText);
		optionBoxes.push_back(optionBox);

		// Set the text to be the first option by default
		selectedText.setString(options[0].second);
	}

	updateColors();
}

// Set the position of the drop down menu
void DropDown::setPosition(sf::Vector2f pos) {
	// Set the position of the drop down menu objects
	position = pos;
	background.setPosition(position);
	foreground.setPosition(sf::Vector2f(position.x + padding, position.y + padding));
	frame.setPosition(position);
	text.setPosition(position.x + 10, position.y + size.y / 4);

	// Set the selected text to be in the center of the foreground
	float x = foreground.getPosition().x + foreground.getSize().x / 2 - selectedText.getGlobalBounds().width / 2;
	float y = foreground.getPosition().y + foreground.getSize().y / 2 - selectedText.getGlobalBounds().height + 1.f;
	selectedText.setPosition(sf::Vector2f(x, y));

	// Set the position of the options
	for (int i = 0; i < options.size(); i++) {
		optionBoxes[i].setPosition(position.x + padding, position.y + size.y + (size.y * i));
		// Set the text in the middle of the box
		float x = optionBoxes[i].getPosition().x + optionBoxes[i].getSize().x / 2 - optionTexts[i].getGlobalBounds().width / 2;
		float y = optionBoxes[i].getPosition().y + optionBoxes[i].getSize().y / 2 - optionTexts[i].getGlobalBounds().height / 2;
		optionTexts[i].setPosition(sf::Vector2f(x, y));
	}
}

// Interact with the drop down menu
void DropDown::interact(sf::RenderWindow& window) {
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

	// If the mouse is over the drop down menu and the mouse is clicked
	if (foreground.getGlobalBounds().contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// If the cooldown is not over
		if (cooldown.getElapsedTime().asSeconds() < 0.2f) {
			return;
		}

		// Set the cooldown
		cooldown.restart();

		// If the drop down menu is open, close it
		if (isOpen) {
			isOpen = false;
			return;
		}

		// If the drop down menu is closed, open it
		isOpen = true;
	}
	else if (!foreground.getGlobalBounds().contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		isOpen = false;
	}
}

// Draw the drop down menu
void DropDown::draw(sf::RenderWindow& window) {
	// Draw the background
	window.draw(background);
	window.draw(foreground);
	window.draw(frame);
	window.draw(text);
	window.draw(selectedText);

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
				selectedText.setString(options[i].second);

				// Run the associated function
				options[i].first();

				// Close the drop down menu
				isOpen = false;
			}
		}
	}
}

// Update the colors of the drop down menu
void DropDown::updateColors() {
	background.setFillColor(backgroundColor);
	foreground.setFillColor(foregroundColor);
	frame.setFillColor(frameColor);
	frame.setOutlineColor(frameColor);
	frame.setOutlineThickness(outlineThickness);
	text.setFillColor(textColor);
	selectedText.setFillColor(textColor);
	for (int i = 0; i < options.size(); i++) {
		optionBoxes[i].setFillColor(optionColor);
		optionBoxes[i].setOutlineColor(optionOutlineColor);
		optionTexts[i].setFillColor(optionTextColor);
	}
}