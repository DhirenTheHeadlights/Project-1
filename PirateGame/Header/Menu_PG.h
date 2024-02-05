#pragma once

/// <summary>
/// The base class for all of the game menus.
/// It is intended to be inherited from and extended to create new menus.
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameStateManager_PG.h"
#include "Interactable_PG.h"
#include "Button_PG.h"
#include "Slider_PG.h"
//#include "TextDisplayBox.h"
//#include "TextInputBox_PG.h"

namespace PirateGame {
	class Menu {
	public:
		Menu(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {} ;
		~Menu() {};

		virtual void setUpMenu() = 0;
		virtual void draw(sf::RenderWindow& window);
		virtual void addInteractableToMenu(std::unique_ptr<Interactable> interactable);
		virtual void addInteractables() = 0;
		virtual void setInteractablePositions() = 0;
		virtual void interactWithMenuItems();

		// Get Menu items
		std::vector<std::unique_ptr<Interactable>>& getInteractables() { return this->interactables; }

	private:
		// Menu variables
		bool isMenuOpen = false;
		bool redraw = false;
		bool positionUpdated = false;
		bool wasButtonPressedLastFrame = false;
		sf::Time totalPauseDuration;

	protected:
		// SFML objects
		sf::Font& font;
		sf::RenderWindow& window;

		// Menu items
		sf::RectangleShape backgroundRect;
		sf::RectangleShape menu;

		// Menu interactables
		std::vector<std::unique_ptr<Interactable>> interactables;

		// Menu Text
		std::string title = "Title not set";
		sf::Text titleText;

		// Menu properties
		sf::Vector2f size = sf::Vector2f(1000.0f, 500.0f);
		sf::Vector2f interactableSize = sf::Vector2f(100.0f, 50.0f);
		float textSize = 20.0f;
		float padding = 10.0f;

		// Menu colors
		sf::Color backgroundColor = sf::Color(0, 0, 0, 200);
		sf::Color textColor = sf::Color(150, 150, 200, 200);
		sf::Color interactableColor = sf::Color(0, 0, 0, 200);
		sf::Color interactableTextColor = sf::Color::White;
		sf::Color hoverColor = sf::Color(0, 0, 0, 100);
	};
}
