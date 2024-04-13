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

		sf::VertexArray createVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color);

		sf::Vector2f normalizeVector(const sf::Vector2f& vector) {
			float length = sqrt(vector.x * vector.x + vector.y * vector.y);
			if (length != 0) {
				return sf::Vector2f(vector.x / length, vector.y / length);
			}
			else {
				return sf::Vector2f(0, 0);
			}
		}

		float distanceBetweenPoints(const sf::Vector2f& point1, const sf::Vector2f& point2) {
			return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
		}

		sf::Vector2f limitVector(const sf::Vector2f& vector, float max) {
			float length = sqrt(vector.x * vector.x + vector.y * vector.y);
			if (length > max) {
				return normalizeVector(vector) * max;
			}
			else {
				return vector;
			}
		}

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
			textSize = globalWindow->getView().getSize().y / textScalingFactor;
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

