#pragma once

/// This class handles all inputs and have easy methods for checking if a key is pressed, toggled, or held.

#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace PirateGame {
    namespace Input {

		void initializeInput();
		void update();
		bool isKeyPressedOnce(sf::Keyboard::Key key);
		bool isKeyToggled(sf::Keyboard::Key key);
		bool isKeyHeld(sf::Keyboard::Key key);
		bool isMouseButtonPressedOnce(sf::Mouse::Button button);
		bool isMouseButtonHeld(sf::Mouse::Button button);

    };
}
