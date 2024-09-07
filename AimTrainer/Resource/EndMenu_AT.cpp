#include "EndMenu_AT.h"

EndMenu::EndMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM) : Menu(window, font), GSM(GSM) {
}

// To set up the menu

void EndMenu::setUpMenu() {
	// Set the title of the menu
	Menu::setTitleString("Thanks for playing!");

	// Lambda wrapper functions for menu interactions
	restartFunc = [this](float value) { this->restart(); };
	returnToTitleFunc = [this](float value) { this->returnToTitleScreen(); };
	exitFunc = [this](float value) { this->exit(); };

	// Add the buttons to the menu
	Menu::addInteractable("Restart", Interactable::Type::Button, restartFunc);
	Menu::addInteractable("Return to title screen", Interactable::Type::Button, returnToTitleFunc);
	Menu::addInteractable("Exit", Interactable::Type::Button, exitFunc);
}

// To draw the menu

void EndMenu::drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
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

	// Toggle the menu if the escape key is pressed
	Menu::toggleMenu(mainClock, crosshair);
	Menu::openMenu(currentEvent, crosshair, mainClock);
}

// Functions for the menu interactions

void EndMenu::restart() const {
	GSM->startGame();
}

void EndMenu::returnToTitleScreen() {
	GSM->openStartMenu();
}

void EndMenu::exit() {
	GSM->exit();
}