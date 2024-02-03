#include "StartMenu_PG.h"

using namespace PirateGame;

// Set up the start menu
void StartMenu::setUpMenu() {
	// Set the title of the menu
	title = "DNS PIRATE GAME ADVENTURE";
	textSize = 100;
	titleText.setPosition(100.f, 100.f);
	titleText.setCharacterSize(static_cast<unsigned int>(textSize));
	titleText.setString(title);

	// Set the size of the menu
	size = sf::Vector2f(400, 400);

	// Set the position of the menu to the bottom right of the window
	sf::Vector2f position = sf::Vector2f(window.getSize().x - size.x - 10, window.getSize().y - size.y - 10);

	// Add the interactables to the menu and set their positions
	addInteractables();
	setInteractablePositions();
}

// Set the position of the interactables
void StartMenu::setInteractablePositions() {
	// Set the positions of the interactables to be in the bottom right of the screen
	sf::Vector2f position = sf::Vector2f(window.getSize().x - size.x - 10, window.getSize().y - size.y - 10);
	for (auto& interactable : interactables) {
		interactable->setPosition(position);
		position.y -= interactableSize.y + 10;
	}
}

// Add the interactables to the menu
void StartMenu::addInteractables() {
	// Create the start button
	std::function<void()> startFunc = [this]() { GSM.changeGameState(GameState::GameLoop); };
	std::unique_ptr<Button> startButton = std::make_unique<Button>(startFunc, font);
	startButton->setUpInteractable(interactableSize);
	startButton->setString("Start");
	addInteractableToMenu(std::move(startButton));

	// Create the settings button
	std::function<void()> settingsFunc = [this]() { GSM.changeGameState(GameState::OptionsMenu); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc, font);
	settingsButton->setUpInteractable(interactableSize);
	settingsButton->setString("Settings");
	addInteractableToMenu(std::move(settingsButton));

	// Create the exit button
	std::function<void()> endFunc = [this]() { GSM.changeGameState(GameState::End); };
	std::unique_ptr<Button> exitButton = std::make_unique<Button>(endFunc, font);
	exitButton->setUpInteractable(interactableSize);
	exitButton->setString("Exit");
	addInteractableToMenu(std::move(exitButton));
}