#pragma once

/// <summary>
/// This class is a child class that represents an interactable drop down 
/// menu to use in the menu classes.
/// </summary>

#include "Interactable_PG.h"

namespace PirateGame {
	class DropDown : public Interactable {
	public:
		DropDown(sf::Font& font, std::vector<std::pair<std::function<void()>, std::string>> options) : Interactable(font), options(options) {};
		~DropDown() {};

		// Set up the drop down menu
		void createInteractable(sf::Vector2f size) override;
		void interact() override;
		void draw() override;
		void openDropDown();
		void updateColors() override;

		// Setters
		void setPosition(sf::Vector2f pos) override;

		// Setters for colors
		void setSelectedColor(sf::Color color) { this->selectedColor = color; }
		void setOptionColor(sf::Color color) { this->optionColor = color; }
		void setOptionOutlineColor(sf::Color color) { this->optionOutlineColor = color; }
		void setOptionTextColor(sf::Color color) { this->optionTextColor = color; }

	private:
		// Variables
		std::vector<std::pair<std::function<void()>, std::string>> options;
		sf::Text selectedText;
		sf::Clock cooldown;
		bool isOpen = false;
		bool wasButtonPressed = false;

		// Drop down menu objects
		std::vector<sf::Text> optionTexts;
		std::vector<sf::RectangleShape> optionBoxes;

		// Colors
		sf::Color selectedColor = sf::Color(100, 100, 150, 200);
		sf::Color optionColor = sf::Color(0, 0, 0, 150);
		sf::Color optionOutlineColor = sf::Color(255, 255, 255);
		sf::Color optionTextColor = sf::Color::White;
		sf::Color optionsBackgroundColor = foregroundColor;
	};
}

