#pragma once

/// This class contains all the global values used throughout the game.
/// It is intended to be included in every file that needs access to these values.

#include <SFML/Graphics.hpp>

#include "Map.h"
#include "InputHandler_PG.h"
#include "GameStateManager_PG.h"
#include "WindController_PG.h"

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

		sf::VertexArray createVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color);

		// Setters
		void setFont(sf::Font font) { this->globalFont = font; }
		void setWindow(sf::RenderWindow* window) {
			if (window == nullptr) {
				std::cerr << "Attempted to set a nullptr window in GlobalValues\n";
			}
			this->globalWindow = window;
		}
		void setClock(sf::Clock clock) { this->globalClock = clock; }
		void setGlobalVolume(float volume) { this->soundVolume = volume; }
		
		// Getters
		sf::Vector2f getMapSize() const { return this->worldMap; }

		float getGlobalVolume() const { return this->soundVolume; }
		int getCellSize() const { return this->cellSize; }

		sf::Font& getFont() { return this->globalFont; }
		sf::RenderWindow* getWindow() { return this->globalWindow; }
		sf::Clock getClock() const { return this->globalClock; }

		Map& getMap() { return this->map; }
		InputHandler& getInputHandler() { return this->inputHandler; }
		GameStateManager& getGSM() { return this->GSM; }
		WindController& getWindController() { return this->windController; }

	private:
		// Private Constructor
		GlobalValues() {};

		// Delete the copy constructor and assignment operator
		GlobalValues(GlobalValues const&) = delete;
		GlobalValues& operator=(GlobalValues const&) = delete;

		// Variables to store the world's values
		Map map;
		sf::Vector2f worldMap = sf::Vector2f(100000, 100000);
		int cellSize = 10;
		float soundVolume = 100;
		WindController windController;

		// Input handler
		InputHandler inputHandler;

		// GameStateManager
		GameStateManager GSM;

		// SFML objects
		sf::Font globalFont;
		sf::RenderWindow* globalWindow = nullptr;
		sf::Clock globalClock;
	};
}

