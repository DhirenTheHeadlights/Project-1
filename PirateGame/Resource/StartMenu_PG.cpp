#include "StartMenu_PG.h"

using namespace PirateGame;

// Set up the start menu
void StartMenu::setUpMenu() {
	// Set the title of the menu
	titleText = sf::Text("DNS PIRATE GAME ADVENTURE", font, static_cast<unsigned int>(100.f));
	titleText.setPosition(static_cast<float>(window->getSize().x / 2) - titleText.getGlobalBounds().getSize().x / 2, static_cast<float>(window->getSize().x / 8u));

	// Add the interactables to the menu and set their positions
	addInteractablesToMenu();
	setInteractablePositions();
}

// Set the position of the interactables
void StartMenu::setInteractablePositions() {
	// Set the positions of the interactables to be in the bottom right of the screen
	sf::Vector2f position = sf::Vector2f(window->getSize().x - buttons[0].getSprite().getGlobalBounds().width - padding,
							window->getSize().y - buttons.size() * (buttons[0].getSprite().getGlobalBounds().height + padding));
	for (auto& button : buttons) {
		button.setPosition(position);
		position.y += button.getSprite().getGlobalBounds().height + padding;
	}
}

// Add the interactables to the menu
void StartMenu::addInteractablesToMenu() {
	// Grab the global game state manager
	GSM = &GlobalGameStateManager::getInstance();

	// Create the start button
	std::function<void()> startFunc = [this]() { GSM->changeGameState(GameState::GameLoop); };
	addButton(sf::Text("Start", font, textSize), GlobalTextureHandler::getInstance().getInteractableTextures().getBlackGrayButton(), buttons, startFunc, interactableScale);

	// Create the settings button
	std::function<void()> settingsFunc = [this]() { std::cout << "opt" << std::endl; GSM->changeGameState(GameState::OptionsMenu); };
	addButton(sf::Text("Options", font, textSize), GlobalTextureHandler::getInstance().getInteractableTextures().getBlackGrayButton(), buttons, settingsFunc, interactableScale);

	// Create the quit button
	std::function<void()> quitFunc = [this]() { GSM->changeGameState(GameState::End); };
	addButton(sf::Text("Quit", font, textSize), GlobalTextureHandler::getInstance().getInteractableTextures().getBlackGrayButton(), buttons, quitFunc, interactableScale);
}

void StartMenu::interactWithMenuItems() {
	// Interact with the buttons
	for (auto& button : buttons) {
		button.interact();
	}
}

void StartMenu::draw() {
	// Draw the title
	window->draw(titleText);

	// Draw the interactables
	for (auto& button : buttons) {
		button.draw();
	}
}

void StartMenu::update() {
	// Update the interactables
	interactWithMenuItems();
}
