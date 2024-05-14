#pragma once

/// This class contains all the global values used throughout the game.
/// It is intended to be included in every file that needs access to these values.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "GlobalFontHandler_PG.h"

namespace PirateGame {
	class GlobalValues {
	public:
		// Public method to access the instance
		static GlobalValues& getInstance() {
			static GlobalValues instance;
			return instance;
		}

		std::string keyToString(sf::Keyboard::Key key);
		std::string buttonToString(sf::Mouse::Button button);

		void printOnlyOnce(const std::string& message) {
			static bool printed = false;
			if (!printed) {
				std::cout << message << std::endl;
				printed = true;
			}
		}

		void displayText(const std::string& text, const sf::Vector2f& position, const sf::Color& color, int size = 30) {
			sf::Text displayText;
			displayText.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
			displayText.setString(text);
			textSize = static_cast<int>(globalWindow->getView().getSize().y) / textScalingFactor;
			displayText.setCharacterSize(textSize);
			displayText.setFillColor(color);
			displayText.setPosition(position);

			// Only draw the text if the scale of the view is smaller than 5
			if (globalWindow->getView().getSize().x / globalWindow->getSize().x < 5) {
				globalWindow->draw(displayText);
			}
		}

		// Setters 
		void setWindow(sf::RenderWindow* window) {
			if (window == nullptr) {
				std::cerr << "Attempted to set a nullptr window in GlobalValues\n";
			}
			this->globalWindow = window;
		}
		void setClock(sf::Clock clock) { this->globalClock = clock; }
		void setShowHUD(bool showHUD) { this->showHUD = showHUD; }
		
		// Getters
		bool getShowHUD() const { return this->showHUD; }
		sf::RenderWindow* getWindow() { return this->globalWindow; }
		sf::Clock getClock() const { return this->globalClock; }
		std::default_random_engine& getRandomEngine() { return randomEngine; }
		int getTextSize() const { return textSize; }

	private:
		// Private Constructor
		GlobalValues() {};

		// Delete the copy constructor and assignment operator
		GlobalValues(GlobalValues const&) = delete;
		GlobalValues& operator=(GlobalValues const&) = delete;

		// Variables to store the world's values
		bool showHUD = true;

		// Text size
		int textSize = 30;
		const int textScalingFactor = 50; // The larger the number, the smaller the text compared to the window size

		// Random engine
		std::default_random_engine randomEngine;

		// SFML objects
		sf::RenderWindow* globalWindow = nullptr;
		sf::Clock globalClock;
	};
}

