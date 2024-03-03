/*#include "IslandMenu_PG.h"

using namespace PirateGame;

void IslandMenu::setUpMenu() {
	size = sf::Vector2f(window->getSize());
	size.x *= 0.7;
	size.y *= 0.6;
	backgroundRect.setSize(size);


	addInteractablesToMenu();
}

void IslandMenu::addInteractablesToMenu() {
	// Add a settings button
	std::function<void()> settingsFunc = [this]() { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc);
	settingsButton->createInteractable(sf::Vector2f(200, 50));
	settingsButton->setString("Settings");
	addInteractableToMenu(std::move(settingsButton));

	// Add an information box for the ship
}

void IslandMenu::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();
	HUDView.setRotation(0);

	// Set the health bar red to be proportional to the health from the ship
	backgroundRect.setSize(sf::Vector2f(healthBarSize.x - (healthBarSize.x * (1 - (ship->getHealth() / ship->getShipProperties().maxHealth))), healthBarSize.y));
	healthBarRed.setSize(healthBarSize);

	// Set the position of the health bar and text
	healthBarGreen.setPosition(HUDView.getCenter().x - healthBarSize.x / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding);
	healthBarRed.setPosition(healthBarGreen.getPosition());

	float healthTextX = healthBarGreen.getPosition().x + healthBarGreen.getSize().x / 2 - healthText.getGlobalBounds().width / 2;
	float healthTextY = healthBarGreen.getPosition().y + healthBarGreen.getSize().y / 2 - healthText.getGlobalBounds().height / 2;
	healthText.setPosition(healthTextX, healthTextY);
	healthText.setString("Health: " + std::to_string(static_cast<int>(100 * ship->getHealth() / ship->getShipProperties().maxHealth)));

	// Set the settings button to be in the top left corner
	interactables[0]->setPosition(sf::Vector2f(HUDView.getCenter().x - window->getSize().x / 2 + padding, healthBarGreen.getPosition().y));

	// Set the mini map to be in the top right corner with padding right by the health bars
	minimap.setPosition(HUDView.getCenter().x + healthBarSize.x / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding);
}

void IslandMenu::draw() {
	// Draw the health bar
	healthBarRed.setPosition(healthBarGreen.getPosition());
	window->draw(healthBarRed);
	window->draw(healthBarGreen);
	window->draw(healthText);

	window->draw(minimap);

	// Draw the interactables
	for (auto& interactable : interactables) {
		//interactable->getBackground().setFillColor(sf::Color::Transparent);
		interactable->getFrame().setFillColor(sf::Color::Transparent);
		interactable->getFrame().setOutlineThickness(0.f);
		interactable->draw();
	}

	// Interact
	setInteractablePositions();
	interactWithMenuItems();
}*/