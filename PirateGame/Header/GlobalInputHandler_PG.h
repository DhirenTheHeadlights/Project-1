#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>

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
        bool isMouseButtonPressedOnce(sf::Mouse::Button button);
        bool isMouseButtonHeld(sf::Mouse::Button button);

    private:
        GlobalInputHandler() {}

        GlobalInputHandler(GlobalInputHandler const&) = delete;
        GlobalInputHandler& operator=(GlobalInputHandler const&) = delete;

        std::unordered_map<sf::Keyboard::Key, bool> lastKeyState;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyState;

        std::unordered_map<sf::Keyboard::Key, bool> toggledKeyState;

        std::unordered_map<sf::Mouse::Button, bool> lastButtonState;
        std::unordered_map<sf::Mouse::Button, bool> currentButtonState;

        sf::Clock cooldown;
        sf::Clock holdCooldown;
        sf::Time cooldownTime = sf::milliseconds(100);

        bool mouseClickedThisFrame = false;
        sf::Mouse::Button clickedButton;
    };
}
