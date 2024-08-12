#pragma once

/// This class is a child class that represents an interactable drop down 
/// menu to use in the menu classes.

#include "Interactable_PG.h"

namespace PirateGame {
	class DropDown : public Interactable {
	public:
		DropDown(std::vector<std::pair<std::function<void()>, std::string>> options) : options(options) {};
		~DropDown() {};

		// Set up the drop down menu
		void customInteractableSetUp() override;
		void interact(sf::RenderWindow* window, GlobalInputHandler* GIH, GlobalSoundManager* GSM) override;
		void draw(sf::RenderWindow* window) override;
		void openDropDown(sf::RenderWindow* window);

		// Setters
		void setPosition(sf::Vector2f pos) override;
		void setOptionsBoxSprite(sf::Texture& texture, sf::Vector2f scale = sf::Vector2f(1.f, 1.f)) {
			for (auto& optionSprite : optionSprites) {
				optionSprite.setTexture(texture);
				optionSprite.setScale(scale);
			}
		}

		// Setters for colors
		void setOptionTextColor(sf::Color color) { this->optionTextColor = color; }

	private:
		// Variables
		std::vector<std::pair<std::function<void()>, std::string>> options;
		sf::Text selectedText;

		bool isOpen = false;
		bool wasButtonPressed = false;

		// Drop down menu objects
		std::vector<sf::Text> optionTexts;
		std::vector<sf::Sprite> optionSprites;

		// Colors
		sf::Color optionTextColor = sf::Color::White;
	};
}

