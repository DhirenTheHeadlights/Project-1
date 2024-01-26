#include "InsertNameMenu.h"

InsertNameMenu::InsertNameMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM) :
	Menu(window, font), GSM(GSM) {
}

// Set up the menu

void InsertNameMenu::setUpMenu() {
	// Set the title of the menu
	Menu::setTitleString("Enter your name");

	// Set the size of the buttons to be longer but thinner
	Menu::setButtonSize(sf::Vector2f(300.f, 50.f));

	// Lambda wrapper functions for menu interactions
	setNameFunc = [this](float value) { this->setName(); };

	// Add the buttons to the menu
	Menu::addInteractable("Type Name Here", Interactable::Type::InputBox, std::nullopt);
	Menu::addInteractable("Continue", Interactable::Type::Button, setNameFunc);
}

// To draw the menu

void InsertNameMenu::drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
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

	// Open the menu
	Menu::toggleMenu(mainClock, crosshair);
	Menu::openMenu(currentEvent, crosshair, mainClock);
}

// Functions for the menu interactions

void InsertNameMenu::setName() {
	GSM->setName(Menu::getInputStringForInteractableI(0));
	GSM->startGame();
}