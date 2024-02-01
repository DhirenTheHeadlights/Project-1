#include "StartMenu_PG.h"

// Set up the start menu
void StartMenu::setUpMenu() {
	// Set the title of the menu
	title = "DNS PIRATE GAME ADVENTURE";

	// Set the size of the menu
	size = sf::Vector2f(400, 400);

	// Set the position of the menu to the bottom right of the window
	sf::Vector2f position = sf::Vector2f(window.getSize().x - size.x - 10, window.getSize().y - size.y - 10);

	// Set up the title text
	titleText.setPosition(100.f, 100.f);
	titleText.setString(title);
	titleText.setCharacterSize(100.0f);

	// Add the interactables to the menu
	addInteractables();

	// Set the position of the interactables
	setInteractablePositions();
}

// Set the position of the interactables
void StartMenu::setInteractablePositions() {
	const float spacing = 20.0f; // Spacing between interactables
	float startY = menu.getPosition().y + spacing; // Start Y position below title with padding

	for (size_t i = 0; i < interactables.size(); i++) {
		float x = window.getSize().x / 2 - interactableSize.x / 2; // Centered X position
		float y = startY + i * (interactableSize.y + spacing); // Stacked Y positions for interactables
		interactables[i]->setPosition(sf::Vector2f(x, y));
	}
}

// Add the interactables to the menu
void StartMenu::addInteractables() {
	// Create the start button
	auto startFunc = [this]() { GSM.changeGameState(GameState::GameLoop); };
	std::unique_ptr<Button> startButton = std::make_unique<Button>(startFunc, font);
	startButton->setString("Start");
	addInteractable(std::move(startButton));

	// Create the settings button
	auto settingsFunc = [this]() { GSM.changeGameState(GameState::OptionsMenu); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc, font);
	settingsButton->setString("Settings");
	addInteractable(std::move(settingsButton));

	// Create the exit button
	auto endFunc = [this]() { GSM.changeGameState(GameState::End); };
	std::unique_ptr<Button> exitButton = std::make_unique<Button>(endFunc, font);
	exitButton->setString("Exit");
	addInteractable(std::move(exitButton));
}
