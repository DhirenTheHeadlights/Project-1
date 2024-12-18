#include "InputHandler_PG.h"

using namespace PirateGame;

void InputHandler::update() {
    // Update the last state to the current state for keys
    lastKeyState = currentKeyState;
    // Update the last state to the current state for mouse buttons
    lastButtonState = currentButtonState;
}

bool InputHandler::isKeyPressedOnce(sf::Keyboard::Key key) {
    // Check if the key is currently pressed and was not pressed in the last frame
    bool isPressedNow = sf::Keyboard::isKeyPressed(key);
    bool wasPressedLastFrame = lastKeyState[key];

    // Update the current state for the next frame
    currentKeyState[key] = isPressedNow;

    // Return true only if the key is pressed now and was not pressed last frame
    return isPressedNow && !wasPressedLastFrame;
}

bool InputHandler::isMouseButtonPressedOnce(sf::Mouse::Button button) {
    // Check if the button is currently pressed and was not pressed in the last frame
    bool isPressedNow = sf::Mouse::isButtonPressed(button);
    bool wasPressedLastFrame = lastButtonState[button];

    // Update the current state for the next frame
    currentButtonState[button] = isPressedNow;

    // Return true only if the button is pressed now and was not pressed last frame
    return isPressedNow && !wasPressedLastFrame;
}