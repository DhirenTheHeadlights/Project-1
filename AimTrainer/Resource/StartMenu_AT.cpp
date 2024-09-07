#include "StartMenu_AT.h"

StartMenu::StartMenu(sf::RenderWindow& window, sf::Font font, GameStateManager* GSM) : 
                    Menu(window, font), GSM(GSM) {
}

// Set up the menu

void StartMenu::setUpMenu() {
    // Set the title of the menu
    Menu::setTitleString("Welcome to Dhiren's Aim Trainer");

	// Lambda wrapper functions for menu interactions
    startFunc = [this](float value) { this->start(); };
    openLeaderBoardFunc = [this](float value) { this->openLeaderBoard(); };
	setNameFunc = [this](float value) { this->setName(); };

	// Add the buttons to the menu
    Menu::addInteractable("Start", Interactable::Type::Button, startFunc);
    Menu::addInteractable("Leaderboard", Interactable::Type::Button, openLeaderBoardFunc);
	Menu::addInteractable("Set Name", Interactable::Type::Button, setNameFunc);
}

// To draw the menu

void StartMenu::drawMenu(sf::Event currentEvent, sf::Clock& mainClock, Crosshair& crosshair) {
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

void StartMenu::start() const {
    GSM->startGame();
}

void StartMenu::openLeaderBoard() {
    GSM->openLeaderBoardMenu();
}

void StartMenu::setName() {
	GSM->openInsertNameMenu();
}