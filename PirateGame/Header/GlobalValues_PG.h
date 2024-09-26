#pragma once

/// This class contains all the global values used throughout the game.

#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "GlobalFontHandler_PG.h"

namespace PirateGame {
	class GlobalValues {
	public:
        GlobalValues() {};

        void loadFromJSON(const std::string& filename);

		std::string keyToString(sf::Keyboard::Key key);
		std::string buttonToString(sf::Mouse::Button button);

        void displayText(const std::string& text, const sf::Vector2f& position, const sf::Color& color, int size = 30) {
            // Generate a key based on text and size for caching
            std::string key = text + "_" + std::to_string(size);

            // Find or create the text object
            sf::Text& displayText = textCache[key];
            if (displayText.getString().isEmpty()) {
				displayText.setFont(Font::timesNewRoman);
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

        void setWindow(sf::RenderWindow* window) {
            if (window == nullptr) {
                std::cerr << "Attempted to set a nullptr window in GlobalValues\n";
            }
            this->globalWindow = window;
        }
        void setShowHUD(bool showHUD) { this->showHUD = showHUD; }
        void setShowInventory(bool showInventory) { this->showInventory = showInventory; }

        bool getShowHUD() const { return this->showHUD; }
        bool getShowInventory() const { return this->showInventory; }
        sf::RenderWindow* getWindow() const { return this->globalWindow; }
        std::default_random_engine& getRandomEngine() { return randomEngine; }
        int getTextSize() const { return textSize; }

    private:
        bool showHUD = true;
        bool showInventory = false;
        int textSize = 30; 
        const int textScalingFactor = 750;
        std::default_random_engine randomEngine;

        sf::RenderWindow* globalWindow = nullptr;

        // Cache for storing text objects
        std::unordered_map<std::string, sf::Text> textCache;

        std::unordered_map<std::string, float> jsonParsed;

        
    };
}