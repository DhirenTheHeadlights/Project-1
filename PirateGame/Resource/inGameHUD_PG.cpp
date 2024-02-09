#include "InGameHUD_PG.h"

using namespace PirateGame;

void InGameHUD::setUpMenu() {
	// Set up the health bar
	healthBarGreen.setSize(sf::Vector2f(200, 20));
	healthBarGreen.setFillColor(sf::Color::Green);
	healthBarRed.setSize(sf::Vector2f(200, 20));
	healthBarRed.setFillColor(sf::Color::Red);
	healthText = sf::Text("Health: " + std::to_string(static_cast<int>(health)), font, 20);

	// Add the interactables
	addInteractables();
	setInteractablePositions();
}

void InGameHUD::addInteractables() {
	// Add a settings button
	std::function<void()> settingsFunc = [this]() { GSM.changeGameState(GameState::OptionsMenu); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc, font);
	settingsButton->setUpInteractable(sf::Vector2f(200, 50));
	settingsButton->setString("Settings");
	addInteractableToMenu(std::move(settingsButton));
}

void InGameHUD::setInteractablePositions() {
	// Get the view position
	sf::Vector2f viewPos = HUDView.getCenter();

	// Set the position of the health bar
	healthBarGreen.setPosition(viewPos.x - 100, viewPos.y - 300);

	// Set the position of the health text
	healthText.setPosition(healthBarGreen.getPosition().x + 10, healthBarGreen.getPosition().y - 30);

	// Set the position of the settings button
	interactables[0]->setPosition(sf::Vector2f(viewPos.x - 100, viewPos.y + 300));
}

void InGameHUD::draw(sf::RenderWindow& window) {
	// Set the view
	//window.setView(HUDView);

	// Draw the health bar
	healthBarRed.setPosition(healthBarGreen.getPosition());
	window.draw(healthBarRed);
	window.draw(healthBarGreen);
	window.draw(healthText);

	// Draw the interactables
	for (auto& interactable : interactables) {
		interactable->draw(window);
	}

	// Interact
	interactWithMenuItems();
}