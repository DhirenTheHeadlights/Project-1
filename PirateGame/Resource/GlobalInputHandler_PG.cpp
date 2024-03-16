#include "GlobalInputHandler_PG.h"

using namespace PirateGame;

void GlobalInputHandler::update() {
    // Update the last state to the current state for keys
    lastKeyState = currentKeyState;
    // Update the last state to the current state for mouse buttons
    lastButtonState = currentButtonState;
}

bool GlobalInputHandler::isKeyPressedOnce(sf::Keyboard::Key key) {
    // Check if the key is currently pressed and was not pressed in the last frame
    bool isPressedNow = sf::Keyboard::isKeyPressed(key);
    bool wasPressedLastFrame = lastKeyState[key];

    // Update the current state for the next frame
    currentKeyState[key] = isPressedNow;

    // Return true only if the key is pressed now and was not pressed last frame and the cooldown has passed
    if (isPressedNow && !wasPressedLastFrame && cooldown.getElapsedTime() > cooldownTime) {
        return true;
		cooldown.restart();
	}
    return false;
}

bool GlobalInputHandler::isKeyToggled(sf::Keyboard::Key key) {
    // Check if the key is currently pressed and was not pressed in the last frame
    bool isPressedNow = sf::Keyboard::isKeyPressed(key);
    bool wasPressedLastFrame = lastKeyState[key];

    // Update the current state for the next frame
    currentKeyState[key] = isPressedNow;

    // If the key is pressed now and was not pressed last frame, toggle the state
    if (isPressedNow && !wasPressedLastFrame && cooldown.getElapsedTime() > cooldownTime) {
        toggledKeyState[key] = !toggledKeyState[key];
        cooldown.restart();
    }

    // Return the toggled state
    return toggledKeyState[key];
}

bool GlobalInputHandler::isMouseButtonPressedOnce(sf::Mouse::Button button) {
    // Check if the button is currently pressed and was not pressed in the last frame
    bool isPressedNow = sf::Mouse::isButtonPressed(button);
    bool wasPressedLastFrame = lastButtonState[button];

    // Update the current state for the next frame
    currentButtonState[button] = isPressedNow;

    // Return true only if the button is pressed now and was not pressed last frame and the cooldown has passed
    if (isPressedNow && !wasPressedLastFrame && cooldown.getElapsedTime() > cooldownTime) {
		return true;
    }
    return false;
}

bool GlobalInputHandler::isMouseButtonToggled(sf::Mouse::Button button) {
    // Check if the button is currently pressed and was not pressed in the last frame
    bool isPressedNow = sf::Mouse::isButtonPressed(button);
    bool wasPressedLastFrame = lastButtonState[button];

    // Update the current state for the next frame
    currentButtonState[button] = isPressedNow;

    // If the button is pressed now and was not pressed last frame, toggle the state
    if (isPressedNow && !wasPressedLastFrame && cooldown.getElapsedTime() > cooldownTime) {
        toggledButtonState[button] = !toggledButtonState[button];
        cooldown.restart();
    }

    // Return the toggled state
    return toggledButtonState[button];
}