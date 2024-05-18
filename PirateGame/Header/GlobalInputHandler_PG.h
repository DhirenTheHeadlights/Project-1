#pragma once

/// This class handles all inputs and have easy methods for checking if a key is pressed, toggled, or held.

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "GlobalInputHandler_PG.h"

namespace PirateGame {
    class GlobalInputHandler {
    public:
        static GlobalInputHandler& getInstance() {
            static GlobalInputHandler instance;
            return instance;
        }

        void update();

        bool isKeyPressedOnce(sf::Keyboard::Key key);
        bool isKeyToggled(sf::Keyboard::Key key);
        bool isKeyHeld(sf::Keyboard::Key key);
        bool isMouseButtonPressedOnce(sf::Mouse::Button button) const;
        bool isMouseButtonHeld(sf::Mouse::Button button);

    private:
        GlobalInputHandler() {
            // Initialize keyboard and mouse states
            for (int key = sf::Keyboard::A; key < sf::Keyboard::KeyCount; ++key) {
                sf::Keyboard::Key k = static_cast<sf::Keyboard::Key>(key);
                lastKeyState[k] = false;
                currentKeyState[k] = false;
                toggledKeyState[k] = false;
                keyToggleTimes[k] = keyCooldownClock.getElapsedTime(); // Initialize cooldown times
            }
        }

        std::unordered_map<sf::Keyboard::Key, bool> lastKeyState;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyState;

        std::unordered_map<sf::Keyboard::Key, bool> toggledKeyState;
        std::unordered_map<sf::Keyboard::Key, sf::Time> keyToggleTimes; // Cooldown times

        std::unordered_map<sf::Mouse::Button, bool> lastButtonState;

        sf::Clock mouseSingleClickCooldown;
        sf::Clock mouseHoldCooldown;
        sf::Clock keyCooldownClock; // General clock for all key cooldowns
        sf::Time cooldownTime = sf::milliseconds(100);

        bool mouseClickedThisFrame = false;
        sf::Mouse::Button clickedButton;
    };
}
