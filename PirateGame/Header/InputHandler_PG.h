#include "SFML\Graphics.hpp"

#include <unordered_map>

// This class handles the input from the user
// The main functionality is to get a singlular input from the user
// per frame and return when needed.

namespace PirateGame {
    class InputHandler {
    public:
        void update();
        bool isKeyPressedOnce(sf::Keyboard::Key key);
        bool isMouseButtonPressedOnce(sf::Mouse::Button button);

    private:
        std::unordered_map<sf::Keyboard::Key, bool> lastKeyState;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyState;
        std::unordered_map<sf::Mouse::Button, bool> lastButtonState;
        std::unordered_map<sf::Mouse::Button, bool> currentButtonState;
    };
}