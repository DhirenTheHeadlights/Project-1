#pragma once

/// Singleton class that handles the input from the user
/// The main functionality is to get a singlular input from the user
/// per frame and return when needed.

#include <SFML\Graphics.hpp>
#include <unordered_map>
#include <iostream>

namespace PirateGame {
    class GlobalInputHandler {
    public:
        // Public method to access the instance
        static GlobalInputHandler& getInstance() {
			static GlobalInputHandler instance;
			return instance;
		}

        void update();

        bool isKeyPressedOnce(sf::Keyboard::Key key);
        bool isMouseButtonPressedOnce(sf::Mouse::Button button);

        bool isKeyToggled(sf::Keyboard::Key key);
        bool isMouseButtonToggled(sf::Mouse::Button button);

        bool isKeyHeld(sf::Keyboard::Key key);
        bool isMouseButtonHeld(sf::Mouse::Button button);

    private:
        // Private Constructor
        GlobalInputHandler() {};

        // Delete the copy constructor and assignment operator
        GlobalInputHandler(GlobalInputHandler const&) = delete;
        GlobalInputHandler& operator=(GlobalInputHandler const&) = delete;

        // Variables
        std::unordered_map<sf::Keyboard::Key, bool> lastKeyState;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyState;
        std::unordered_map<sf::Mouse::Button, bool> lastButtonState;
        std::unordered_map<sf::Mouse::Button, bool> currentButtonState;

        std::unordered_map<sf::Keyboard::Key, bool> toggledKeyState;
        std::unordered_map<sf::Mouse::Button, bool> toggledButtonState;

        sf::Clock cooldown;
        sf::Time cooldownTime = sf::milliseconds(100);

        sf::Clock holdClock;
        sf::Time holdUpdate = sf::milliseconds(100); // Update every 100ms
    };
}