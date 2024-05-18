#include "GlobalInputHandler_PG.h"

using namespace PirateGame;
  
void GlobalInputHandler::update() {
    currentKeyState = lastKeyState; // Update the current key state

    sf::Time currentTime = keyCooldownClock.getElapsedTime();

    // Handle mouse input states and clicks
    bool anyButtonClicked = false;
    for (int button = sf::Mouse::Left; button <= sf::Mouse::XButton2; ++button) {
        sf::Mouse::Button btn = static_cast<sf::Mouse::Button>(button);
        bool isPressedNow = sf::Mouse::isButtonPressed(btn);
        bool wasPressedLast = lastButtonState[btn];
        lastButtonState[btn] = isPressedNow;

        if (isPressedNow && !wasPressedLast) {
            if (mouseSingleClickCooldown.getElapsedTime() > cooldownTime) {
                clickedButton = btn;
                mouseClickedThisFrame = true;
                mouseSingleClickCooldown.restart();
                anyButtonClicked = true;
                break; // Register the first valid click within the cooldown period
            }
        }
    }

    if (!anyButtonClicked) {
        mouseClickedThisFrame = false;
    }

    // Update key states
    for (int key = sf::Keyboard::A; key < sf::Keyboard::KeyCount; ++key) {
        sf::Keyboard::Key k = static_cast<sf::Keyboard::Key>(key);
        bool isPressedNow = sf::Keyboard::isKeyPressed(k);
        bool wasPressedLast = lastKeyState[k];

        if (isPressedNow && !wasPressedLast && currentTime - keyToggleTimes[k] > cooldownTime) {
            toggledKeyState[k] = !toggledKeyState[k];
            keyToggleTimes[k] = currentTime; // Update the last toggle time
        }

        lastKeyState[k] = isPressedNow;
    }
}

bool GlobalInputHandler::isKeyPressedOnce(sf::Keyboard::Key key) {
    return currentKeyState[key] && !lastKeyState[key];
}

bool GlobalInputHandler::isKeyToggled(sf::Keyboard::Key key) {
    return toggledKeyState[key];
}

bool GlobalInputHandler::isKeyHeld(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool GlobalInputHandler::isMouseButtonPressedOnce(sf::Mouse::Button button) const {
    return mouseClickedThisFrame && clickedButton == button;
}

bool GlobalInputHandler::isMouseButtonHeld(sf::Mouse::Button button) {
    if (sf::Mouse::isButtonPressed(button) && mouseHoldCooldown.getElapsedTime() > cooldownTime) {
        mouseHoldCooldown.restart();
        return true;
    }
    return false;
}
