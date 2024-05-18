#include "GlobalInputHandler_PG.h"

using namespace PirateGame;

void GlobalInputHandler::update() {
    // Update key states
    lastKeyState = currentKeyState;

    // Handle mouse button states and detect clicks
    bool anyButtonClicked = false;
    for (int button = sf::Mouse::Left; button <= sf::Mouse::XButton2; ++button) {
        sf::Mouse::Button btn = static_cast<sf::Mouse::Button>(button);
        bool isPressedNow = sf::Mouse::isButtonPressed(btn);
        bool wasPressedLast = lastButtonState[btn];
        lastButtonState[btn] = isPressedNow;

        // Check for transition from not pressed to pressed and handle cooldown
        if (isPressedNow && !wasPressedLast) {
            if (cooldown.getElapsedTime() > cooldownTime) {
                clickedButton = btn;
                mouseClickedThisFrame = true;
                cooldown.restart();
                anyButtonClicked = true;
                break; // Register the first valid click within the cooldown period
            }
        }
    }

    // If no button clicked, update mouseClickedThisFrame status
    if (!anyButtonClicked) {
        mouseClickedThisFrame = false;
    }
}

bool GlobalInputHandler::isMouseButtonPressedOnce(sf::Mouse::Button button) {
    return mouseClickedThisFrame && clickedButton == button;
}

bool GlobalInputHandler::isMouseButtonHeld(sf::Mouse::Button button) {
    if (sf::Mouse::isButtonPressed(button) && holdCooldown.getElapsedTime() > cooldownTime) {
        holdCooldown.restart();
        return true;
    }
    return false;
}

bool GlobalInputHandler::isKeyPressedOnce(sf::Keyboard::Key key) {
    bool isPressedNow = sf::Keyboard::isKeyPressed(key);
    bool wasPressedLastFrame = lastKeyState[key];
    currentKeyState[key] = isPressedNow;
    if (isPressedNow && !wasPressedLastFrame) {
        lastKeyState[key] = true;
        return true;
    }
    lastKeyState[key] = isPressedNow;
    return false;
}

bool GlobalInputHandler::isKeyToggled(sf::Keyboard::Key key) {
    bool isPressedNow = sf::Keyboard::isKeyPressed(key);
    if (isPressedNow && !lastKeyState[key]) {
        toggledKeyState[key] = !toggledKeyState[key];
        lastKeyState[key] = true;
        return toggledKeyState[key];
    }
    lastKeyState[key] = isPressedNow;
    return toggledKeyState[key];
}

bool GlobalInputHandler::isKeyHeld(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}