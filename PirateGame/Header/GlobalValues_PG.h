#pragma once

/// This class contains all the global values used throughout the game.
/// It is intended to be included in every file that needs access to these values.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <unordered_map>

#include "GlobalFontHandler_PG.h"
#include "VectorMath.h"

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

        void displayText(const std::string& text, const sf::Vector2f& position, const sf::Color& color, int size = 30) {
            // Generate a key based on text and size for caching
            std::string key = text + "_" + std::to_string(size);

            // Find or create the text object
            sf::Text& displayText = textCache[key];
            if (displayText.getString().isEmpty()) {
                displayText.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
                displayText.setString(text);
                displayText.setCharacterSize(size);
                displayText.setFillColor(color);
                displayText.setPosition(position);
            }

            // Update the position and size of the text
            displayText.setPosition(position);
            textSize = size * static_cast<int>(globalWindow->getView().getSize().y) / textScalingFactor;
            displayText.setCharacterSize(textSize);

            // Only draw the text if the view scale is small enough
            if (globalWindow->getView().getSize().x / globalWindow->getSize().x < 5) {
                globalWindow->draw(displayText);
            }
        }

        void printTimeSinceLastCall() {
			std::cout << "Time since last call: " << timeSinceLastCall.getElapsedTime().asSeconds() << " seconds\n";  
			timeSinceLastCall.restart();
		}

        void setWindow(sf::RenderWindow* window) {
            if (window == nullptr) {
                std::cerr << "Attempted to set a nullptr window in GlobalValues\n";
            }
            this->globalWindow = window;
        }
        void setClock(sf::Clock clock) { this->globalClock = clock; }
        void setShowHUD(bool showHUD) { this->showHUD = showHUD; }
        void setShowInventory(bool showInventory) { this->showInventory = showInventory; }

        bool getShowHUD() const { return this->showHUD; }
        bool getShowInventory() const { return this->showInventory; }
        sf::RenderWindow* getWindow() { return this->globalWindow; }
        sf::Clock& getGlobalClock() { return this->globalClock; }
        std::default_random_engine& getRandomEngine() { return randomEngine; }
        int getTextSize() const { return textSize; }

    private:
        GlobalValues() {};
        GlobalValues(GlobalValues const&) = delete;
        GlobalValues& operator=(GlobalValues const&) = delete;

        bool showHUD = true;
        bool showInventory = false;
        int textSize = 30;
        const int textScalingFactor = 750;
        std::default_random_engine randomEngine;
        sf::RenderWindow* globalWindow = nullptr;
        sf::Clock globalClock;

        // Cache for storing text objects
        std::unordered_map<std::string, sf::Text> textCache;

        // Time since last call
        sf::Clock timeSinceLastCall;
    };
}