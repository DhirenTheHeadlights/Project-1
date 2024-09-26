#pragma once

/// This class contains all the global values used throughout the game.

#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>


namespace PirateGame {
	namespace Globals {

		inline bool showHUD = true;
		inline bool showInventory = false;
		inline sf::RenderWindow* window = nullptr;

		inline std::unordered_map<std::string, float> jsonParsed;

		inline sf::Vector2f getGlobalMousePosition() {return static_cast<sf::Vector2f>(sf::Mouse::getPosition(*Globals::window));}
		inline sf::Vector2f getGlobalWorldPosition() {return Globals::window->mapPixelToCoords(sf::Mouse::getPosition(*Globals::window));}

        void loadFromJSON(const std::string& filename);

		std::string keyToString(sf::Keyboard::Key key);

		std::string buttonToString(sf::Mouse::Button button);




        
    };
}