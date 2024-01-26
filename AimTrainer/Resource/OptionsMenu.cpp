#include "OptionsMenu.h"

OptionsMenu::OptionsMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM) : Menu(window, font), GSM(GSM) {
}

// Set up the menu

void OptionsMenu::setUpMenu() {
	// Set the title of the menu
	Menu::setTitleString("Options");

	// Set the length, width and text size of the buttons to be larger than default
	Menu::setButtonSize(sf::Vector2f(400.0f, 200.0f));

	// Lambda wrapper functions for menu interactions
	maxTargetFunc = [this](float value) { this->maxTarget(); };
	setTargetSizeFunc = [this](float value) { this->setTargetSize(value); };
	setMouseSensitivityFunc = [this](float value) { this->setMouseSensitivity(value); };

	// Add the buttons to the menu
	Menu::addInteractable("Change target size", Interactable::Type::Slider, setTargetSizeFunc);
	Menu::addInteractable("Change mouse sensitivity", Interactable::Type::Slider, setMouseSensitivityFunc);
}

// To draw the menu
void OptionsMenu::drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
	// Make sure the menu is drawn once
	if (Menu::needsRedraw()) {
		Menu::drawMenu(currentEvent);
		Menu::resetRedrawFlag();
	}

	// Set individual colors
	Menu::setBackgroundColor(backgroundColor);
	Menu::setTextColor(textColor);
	Menu::setButtonColor(buttonColor);
	Menu::setSliderColor(sliderTrackColor);
	Menu::setSliderComplementColor(sliderThumbColor);
	Menu::setHoverColor(interactableHoverColor);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !wasTabPressedLastFrame) {
		GSM->changeGameState(GameState::OptionsMenu);
		Menu::toggleMenu(mainClock, crosshair);
		wasTabPressedLastFrame = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		wasTabPressedLastFrame = false;
	}

	// Toggle the menu if the escape key is pressed
	Menu::openMenu(currentEvent, crosshair, mainClock);
}

// Functions for the menu interactions

void OptionsMenu::maxTarget() {
	// to do
}

void OptionsMenu::setTargetSize(float newSize) {
	
}

void OptionsMenu::setMouseSensitivity(float sens) {
	
}