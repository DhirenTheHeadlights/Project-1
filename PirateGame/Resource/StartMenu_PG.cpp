#include "StartMenu_PG.h"
#include "GameState_PG.h"

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
	// Create the start button
	std::function<void()> startFunc = [this]() { GameState::changeGameState(GameState::State::GameLoop); };
	addButton(sf::Text("Start", font, textSize), Textures::interactableTextures.getBlackGrayButton(), buttons, startFunc, interactableScale);

	// Create the settings button
	std::function<void()> settingsFunc = [this]() { std::cout << "opt" << std::endl; GameState::changeGameState(GameState::State::OptionsMenu); };
	addButton(sf::Text("Options", font, textSize), Textures::interactableTextures.getBlackGrayButton(), buttons, settingsFunc, interactableScale);

	// Create the quit button
	std::function<void()> quitFunc = [this]() { GameState::changeGameState(GameState::State::End); };
	addButton(sf::Text("Quit", font, textSize), Textures::interactableTextures.getBlackGrayButton(), buttons, quitFunc, interactableScale);
}

void StartMenu::interactWithMenuItems() {
	// Interact with the buttons
	for (auto& button : buttons) {
		button.interact(window,  context.GSM.get());
	}
}

void StartMenu::draw() {
	// Draw the title
	window->draw(titleText);

	// Draw the interactables
	for (auto& button : buttons) {
		button.draw(window);
	}
}

void StartMenu::update() {
	// Update the interactables
	interactWithMenuItems();
}
