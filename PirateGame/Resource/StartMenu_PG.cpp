#include "StartMenu_PG.h"

using namespace PirateGame;

// Set up the start menu
void StartMenu::setUpMenu() {
	// Set up the background sprite


	// Set the title of the menu
	titleText = sf::Text("DNS PIRATE GAME ADVENTURE", font, static_cast<unsigned int>(100.f));
	titleText.setPosition(window->getSize().x / 2 - titleText.getGlobalBounds().getSize().x / 2, window->getSize().x / 8);

	// Add the interactables to the menu and set their positions
	addInteractablesToMenu();
	setInteractablePositions();
}

// Set the position of the interactables
void StartMenu::setInteractablePositions() {
	// Set the positions of the interactables to be in the bottom right of the screen
	sf::Vector2f position = sf::Vector2f(window->getSize().x - interactables[0]->getSprite().getGlobalBounds().width - padding,
							window->getSize().y - interactables.size() * (interactables[0]->getSprite().getGlobalBounds().height + padding));
	for (auto& interactable : interactables) {
		interactable->setPosition(position);
		position.y += interactable->getSprite().getGlobalBounds().height + padding;
	}
}

// Add the interactables to the menu
void StartMenu::addInteractablesToMenu() {
	// Grab the global game state manager
	GSM = &GlobalValues::getInstance().getGSM();

	// Create the start button
	std::function<void()> startFunc = [this]() { GSM->changeGameState(GameState::GameLoop); };
	setUpInteractable(startFunc, sf::Text("Start", font, textSize), interactableScale);

	// Create the settings button
	std::function<void()> settingsFunc = [this]() { GSM->changeGameState(GameState::OptionsMenu); };
	setUpInteractable(settingsFunc, sf::Text("Settings", font, textSize), interactableScale);

	// Create the quit button
	std::function<void()> quitFunc = [this]() { GSM->changeGameState(GameState::End); };
	setUpInteractable(quitFunc, sf::Text("Quit", font, textSize), interactableScale);
}

// Helper interactable set up function
void StartMenu::setUpInteractable(std::function<void()> function, sf::Text name, sf::Vector2f scale) {
	std::unique_ptr<Button> button = std::make_unique<Button>(function);
	button->createInteractable(GlobalTextureHandler::getInstance().getInteractableTextures().getBlackGrayButton(), name, scale);
	addInteractableToMenu(std::move(button));
}
