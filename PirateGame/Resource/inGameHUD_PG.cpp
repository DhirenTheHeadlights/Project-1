#include "InGameHUD_PG.h"

using namespace PirateGame;

void InGameHUD::setUpMenu() {
	healthBarSize = sf::Vector2f(window->getSize().x - 400 - padding * 2, 50);

	// Set up the health bar
	healthBarGreen.setSize(sf::Vector2f(healthBarSize));
	healthBarGreen.setFillColor(sf::Color::Green);
	healthBarRed.setSize(sf::Vector2f(healthBarSize));
	healthBarRed.setFillColor(sf::Color::Red);
	healthText = sf::Text("Health: " + std::to_string(static_cast<int>(healthFraction)), font, 20);
	healthText.setFillColor(sf::Color::Black);

	// Set up the minimap
	minimap.setSize(sf::Vector2f(200, 200));
	minimap.setFillColor(sf::Color::Blue);


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

	// Add an information box for the ship
}
 
void InGameHUD::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();
	HUDView.setRotation(0);

	// Set the health bar red to be proportional to the health from the ship
	healthBarRed.setSize(sf::Vector2f(healthBarSize.x - (healthBarSize.x * healthFraction / 100), healthBarSize.y));

	// Set the position of the health bar and text
	healthBarGreen.setPosition(HUDView.getCenter().x - healthBarSize.x / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding);

	float healthTextX = healthBarGreen.getPosition().x + healthBarGreen.getSize().x / 2 - healthText.getGlobalBounds().width / 2;
	float healthTextY = healthBarGreen.getPosition().y + healthBarGreen.getSize().y / 2 - healthText.getGlobalBounds().height / 2;
	healthText.setPosition(healthTextX, healthTextY);
	healthText.setString("Health: " + std::to_string(static_cast<int>(healthFraction)));

	// Set the settings button to be in the top left corner
	interactables[0]->setPosition(sf::Vector2f(HUDView.getCenter().x - window->getSize().x / 2 + padding, HUDView.getCenter().y - window->getSize().x));

	// Set the mini map to be in the top right corner with padding right by the health bars
	minimap.setPosition(HUDView.getCenter().x + healthBarSize.x / 2 + padding, HUDView.getCenter().y - window->getSize().y / 2 + padding);
}

void InGameHUD::draw() {
	// Draw the health bar
	healthBarRed.setPosition(healthBarGreen.getPosition());
	window->draw(healthBarRed);
	window->draw(healthBarGreen);
	window->draw(healthText);

	window->draw(minimap);

	// Draw the interactables
	for (auto& interactable : interactables) {
		interactable->draw();
	}

	// Interact
	setInteractablePositions();
	interactWithMenuItems();															
}