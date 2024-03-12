#include "DropDown_PG.h"

using namespace PirateGame;

// Set up the drop down menu
void DropDown::customInteractableSetUp() {
	// Set the selected text
	selectedText = sf::Text(options[0].second, font, static_cast<unsigned int>(defaultTextSize));

	// Set up the drop down menu
	for (int i = 0; i < options.size(); i++) {
		// Set up the option text
		sf::Text optionText(options[i].second, font, static_cast<unsigned int>(defaultTextSize));
		optionText.setFillColor(optionTextColor);
		optionTexts.push_back(optionText);

		sf::Sprite optionBox;
		optionSprites.push_back(optionBox);
	}
}

// Set the position of the drop down menu
void DropDown::setPosition(sf::Vector2f pos) {
	// Set the position of the drop down menu objects
	sprite.setPosition(pos);
	text.setPosition(pos.x - text.getGlobalBounds().getSize().x, pos.y - text.getGlobalBounds().getSize().y / 2);

	// Set the selected text to be in the center of the sprite
	selectedText.setPosition(pos.x - selectedText.getGlobalBounds().getSize().x / 2, pos.y - selectedText.getGlobalBounds().getSize().y / 2);

	// Set the position of the options
	for (int i = 0; i < options.size(); i++) {
		float spriteHeight = optionSprites[i].getGlobalBounds().getSize().y;
		optionSprites[i].setPosition(pos.x, pos.y + spriteHeight + (spriteHeight * i));
		float textHeight = optionTexts[i].getGlobalBounds().getSize().y;
		optionTexts[i].setPosition(pos.x, pos.y + spriteHeight + (spriteHeight * i));
	}
}

// Interact with the drop down menu
void DropDown::interact() {
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));

	// Check if the mouse is clicked.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// Check if the cooldown has elapsed.
		if (!(cooldown.getElapsedTime().asSeconds() > cooldownTime.asSeconds())) return;
			
		// Restart the cooldown.
		cooldown.restart();

		// Toggle the open state if the mouse is over the sprite.
		if (sprite.getGlobalBounds().contains(mousePos)) {
			isOpen = !isOpen;
		}
		else {
			// Close the menu if clicking outside the drop-down.
			isOpen = false;
		}
	}
}


// Draw the drop down menu
void DropDown::draw() {
	// Draw the background
	window->draw(sprite);
	window->draw(text);
	window->draw(selectedText);

	// If the drop down menu is open, open the drop down menu
	if (isOpen) {
		openDropDown();
	}
}

// Open the drop down menu if needed
void DropDown::openDropDown() {
	// Draw the options
	for (int i = 0; i < options.size(); i++) {
		window->draw(optionSprites[i]);
		window->draw(optionTexts[i]);
	}

	// If the mouse is over an option
	for (int i = 0; i < options.size(); i++) {
		if (optionSprites[i].getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window)))) {
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
