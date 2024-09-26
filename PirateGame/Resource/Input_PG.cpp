#include "Input_PG.h"

using namespace PirateGame;

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
sf::Mouse::Button clickedButton = sf::Mouse::Button::ButtonCount;

void Input::initializeInput() {
	// Initialize keyboard and mouse states
	for (int key = sf::Keyboard::A; key < sf::Keyboard::KeyCount; ++key) {
		auto k = static_cast<sf::Keyboard::Key>(key);
		lastKeyState[k] = false;
		currentKeyState[k] = false;
		toggledKeyState[k] = false;
		keyToggleTimes[k] = keyCooldownClock.getElapsedTime(); // Initialize cooldown times
	}
}

void Input::update() {
    currentKeyState = lastKeyState; // Update the current key state

    const sf::Time currentTime = keyCooldownClock.getElapsedTime();

    // Handle mouse input states and clicks
    bool anyButtonClicked = false;
    for (int button = sf::Mouse::Left; button <= sf::Mouse::XButton2; ++button) {
	    auto btn = static_cast<sf::Mouse::Button>(button);
        const bool isPressedNow = sf::Mouse::isButtonPressed(btn);
        const bool wasPressedLast = lastButtonState[btn];
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
        const bool isPressedNow = sf::Keyboard::isKeyPressed(k);
        const bool wasPressedLast = lastKeyState[k];

        if (isPressedNow && !wasPressedLast && currentTime - keyToggleTimes[k] > cooldownTime) {
            toggledKeyState[k] = !toggledKeyState[k];
            keyToggleTimes[k] = currentTime; // Update the last toggle time
        }

        lastKeyState[k] = isPressedNow;
    }
}

bool Input::isKeyPressedOnce(sf::Keyboard::Key key) {
    return currentKeyState[key] && !lastKeyState[key];
}

bool Input::isKeyToggled(sf::Keyboard::Key key) {
    return toggledKeyState[key];
}

bool Input::isKeyHeld(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool Input::isMouseButtonPressedOnce(sf::Mouse::Button button) {
    return mouseClickedThisFrame && clickedButton == button;
}

bool Input::isMouseButtonHeld(sf::Mouse::Button button) {
    if (sf::Mouse::isButtonPressed(button) && mouseHoldCooldown.getElapsedTime() > cooldownTime) {
        mouseHoldCooldown.restart();
        return true;
    }
    return false;
}
