#include "InGameHUD_PG.h"

using namespace PirateGame;

void InGameHUD::setUpMenu() {
	// Set up the health bar
	healthBarGreen.setSize(sf::Vector2f(200, 20));
	healthBarGreen.setFillColor(sf::Color::Green);
	healthBarRed.setSize(sf::Vector2f(200, 20));
	healthBarRed.setFillColor(sf::Color::Red);
	healthText = sf::Text("Health: " + std::to_string(static_cast<int>(health)), font, 20);
	healthText.setFillColor(sf::Color::Black);

	// Add the interactables
	addInteractables();
}

void InGameHUD::addInteractables() {
	// Add a settings button
	std::function<void()> settingsFunc = [this]() { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc, font);
	settingsButton->setUpInteractable(sf::Vector2f(200, 50));
	settingsButton->setString("Settings");
	addInteractableToMenu(std::move(settingsButton));
}
 
void InGameHUD::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();
	HUDView.setRotation(0);

	// Set the position of the health bar to be in the bottom right corner of the screen
	healthBarGreen.setPosition(HUDView.getCenter().x + window->getSize().x / 2 - 220, HUDView.getCenter().y + window->getSize().y / 2 - 30);

	// Set the position of the health text to be centered in the health bar
	healthText.setPosition(healthBarGreen.getPosition().x + 50, healthBarGreen.getPosition().y - 1);

	// Set the position of the settings button
	interactables[0]->setPosition(sf::Vector2f(HUDView.getCenter().x - 100, HUDView.getCenter().y + 300));
}

void InGameHUD::draw() {
	// Draw the health bar
	healthBarRed.setPosition(healthBarGreen.getPosition());
	window->draw(healthBarRed);
	window->draw(healthBarGreen);
	window->draw(healthText);

	// Draw the interactables
	for (auto& interactable : interactables) {
		interactable->draw();
	}

	// Interact
	setInteractablePositions();
	interactWithMenuItems();															
}