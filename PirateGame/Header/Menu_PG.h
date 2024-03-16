#pragma once

/// The base class for all of the game menus.
/// It is intended to be inherited from and extended to create new menus.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "GlobalValues_PG.h"
#include "GlobalGameStateManager_PG.h"
#include "GlobalFontHandler_PG.h"

#include "Interactable_PG.h"
#include "Button_PG.h"
#include "Slider_PG.h"
#include "TextDisplayBox_PG.h"
#include "TextInpuBox_PG.h"
#include "DropDown_PG.h"

namespace PirateGame {
	class Menu {
	public:
		Menu() {
			if (GlobalValues::getInstance().getWindow() == nullptr) {
				std::cerr << "Attempted to set a nullptr window in Menu\n";
			}
			this->window = GlobalValues::getInstance().getWindow();
		}

		~Menu() {};

		virtual void setUpMenu() = 0;
		virtual void draw();
		virtual void addInteractableToMenu(std::unique_ptr<Interactable> interactable);
		virtual void addInteractablesToMenu() = 0;
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
		sf::Font& font = *GlobalFontHandler::getInstance().getGlobalFont();
		sf::RenderWindow* window = nullptr;

		// Menu items
		sf::Sprite background;
		sf::Sprite menu;

		// Menu interactables
		std::vector<std::unique_ptr<Interactable>> interactables;

		// Menu Text
		std::string title = "Title not set";
		sf::Text titleText;

		// Menu properties
		sf::Vector2f backgroundScale = { 1, 1 };
		sf::Vector2f menuScale = { 1, 1 };

		sf::Color textColor = sf::Color(0, 0, 0, 200); // Black
		
		unsigned textSize = 20u;
		unsigned interactableTextSize = 10u;
		float padding = 10.0f;

		// GameStateManager
		GlobalGameStateManager* GSM = nullptr;

		// Helper function to convert a float to a string with a set number of decimal places
		std::string floatToString(float number, int decimalPlaces = 2) {
			std::stringstream stream;
			stream << std::fixed << std::setprecision(decimalPlaces) << number;
			return stream.str();
		}
	};
}
