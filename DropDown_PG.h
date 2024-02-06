#pragma once

/// <summary>
/// This class is a child class that represents an interactable drop down 
/// menu to use in the menu classes.
/// </summary>

#include "Interactable_PG.h"

namespace PirateGame {
	class DropDown : public Interactable {
	public:
		DropDown(sf::Font& font, std::vector<std::string> options) : Interactable(font), options(options) {};
		~DropDown() {};

		// Set up the drop down menu
		void setUpInteractable(sf::Vector2f size) override;
		void interact(sf::RenderWindow& window) override;
		void draw(sf::RenderWindow& window) override;
		void openDropDown(sf::RenderWindow& window);

		// Setters
		void setPosition(sf::Vector2f pos) override;

		// Getters
		std::string getSelectedOption() { return selectedOption; }
	private:
		// Variables
		std::vector<std::string> options;
		std::string selectedOption;
		bool isOpen = false;

		// Drop down menu objects
		std::vector<sf::Text> optionTexts;
		std::vector<sf::RectangleShape> optionBoxes;

		// Objects
		sf::RectangleShape arrowBox;
		sf::ConvexShape arrow;
	};
}

