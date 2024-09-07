#include "DropDown_PG.h"

using namespace PirateGame;

// Set up the drop down menu
void DropDown::customInteractableSetUp() {
	// Set the selected text
	selectedText = sf::Text(options[0].second, *text.getFont(), static_cast<unsigned int>(text.getCharacterSize()));

	// Set up the drop down menu
	for (int i = 0; i < options.size(); i++) {
		// Set up the option text
		sf::Text optionText(options[i].second, *text.getFont(), static_cast<unsigned int>(text.getCharacterSize()));
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

	text.setOrigin(0.f, 0.f);
	text.setPosition(pos.x + sprite.getGlobalBounds().getSize().x / 2 - text.getLocalBounds().getSize().x / 2, 
					 pos.y + sprite.getGlobalBounds().getSize().y / 2 - text.getLocalBounds().getSize().y / 2);

	// Set the selected text to be in the center of the sprite
	selectedText.setOrigin(0.f, 0.f);
	selectedText.setPosition(pos.x + sprite.getGlobalBounds().getSize().x / 2 - selectedText.getLocalBounds().getSize().x / 2,
							 pos.y + sprite.getGlobalBounds().getSize().y / 2 - selectedText.getLocalBounds().getSize().y / 2);

	// Set the position of the options
	for (int i = 0; i < options.size(); i++) {
		float spriteHeight = optionSprites[i].getGlobalBounds().getSize().y;
		optionSprites[i].setPosition(pos.x, pos.y + spriteHeight + (spriteHeight * i));
		float textHeight = optionTexts[i].getGlobalBounds().getSize().y;
		float spriteWidth = optionSprites[i].getGlobalBounds().getSize().x;
		optionTexts[i].setPosition(pos.x + spriteWidth / 2 - optionTexts[i].getGlobalBounds().getSize().x / 2, 
								  pos.y + spriteHeight + (spriteHeight * i) + spriteHeight / 2 - textHeight / 2);
	}
}

// Interact with the drop down menu
void DropDown::interact(sf::RenderWindow* window, GlobalInputHandler* GIH, GlobalSoundManager* GSM) {
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window));

	// Check if the mouse is clicked.
	if (GIH->isMouseButtonPressedOnce(sf::Mouse::Left)) {
		// Toggle the open state if the mouse is over the sprite.
		if (sprite.getGlobalBounds().contains(mousePos)) {
			GSM->playSound(SoundId::Select);
			isOpen = !isOpen;
		}
		else {
			// Close the menu if clicking outside the drop-down.
			isOpen = false;
		}
	}

	// If the drop down menu is open, check if an option is clicked.
	if (isOpen) {
		for (int i = 0; i < options.size(); i++) {
			if (optionSprites[i].getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))) && GIH->isMouseButtonPressedOnce(sf::Mouse::Left)) {
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


// Draw the drop down menu
void DropDown::draw(sf::RenderWindow* window) {
	// Draw the background
	window->draw(sprite);
	window->draw(text);
	window->draw(selectedText);

	// If the drop down menu is open, open the drop down menu
	if (isOpen) {
		openDropDown(window);
	}
}

// Open the drop down menu if needed
void DropDown::openDropDown(sf::RenderWindow* window) const {
	// Draw the options
	for (int i = 0; i < options.size(); i++) {
		window->draw(optionSprites[i]);
		window->draw(optionTexts[i]);
	}
}
