#pragma once

/// This class contains all the global values used throughout the game.
/// It is intended to be included in every file that needs access to these values.

#include <SFML/Graphics.hpp>

#include "Map.h"
#include "InputHandler_PG.h"

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

		// Setters
		void setFont(sf::Font font) { this->globalFont = font; }
		void setWindow(sf::RenderWindow* window) {
			if (window == nullptr) {
				std::cerr << "Attempted to set a nullptr window in GlobalValues\n";
			}
			this->globalWindow = window;
		}
		void setClock(sf::Clock clock) { this->globalClock = clock; }
		
		// Getters
		sf::Vector2f getMapSize() const { return this->worldMap; }
		Map& getMap() { return this->map; }
		int getCellSize() const { return this->cellSize; }
		sf::Font& getFont() { return this->globalFont; }
		sf::RenderWindow* getWindow() { return this->globalWindow; }
		sf::Clock getClock() const { return this->globalClock; }
		InputHandler& getInputHandler() { return this->inputHandler; }

	private:
		// Private Constructor
		GlobalValues() {};

		// Delete the copy constructor and assignment operator
		GlobalValues(GlobalValues const&) = delete;
		GlobalValues& operator=(GlobalValues const&) = delete;

		// Variables to store the world's values
		Map map;
		sf::Vector2f worldMap = sf::Vector2f(10000, 10000);
		int cellSize = 10;

		// Input handler
		InputHandler inputHandler;

		// SFML objects
		sf::Font globalFont;
		sf::RenderWindow* globalWindow = nullptr;
		sf::Clock globalClock;
	};
}

