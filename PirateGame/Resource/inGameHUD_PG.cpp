#include "InGameHUD_PG.h"
#include "GameState_PG.h"

using namespace PirateGame;

void InGameHUD::setUpMenu() {
	healthText = sf::Text("Health: " + std::to_string(static_cast<int>(healthFraction)), font, 20);
	healthText.setFillColor(sf::Color::Black);

	interactableTextSize = 20;

	// Set up the minimap
	minimap.setMinimapRadius(minimapSize);

	addInteractablesToMenu();
}

void InGameHUD::addInteractablesToMenu() {
	// Add a settings button
	std::function<void()> settingsFunc = [this]() { GameState::changeGameState(GameState::State::OptionsMenu); };
	Button settingsButton(settingsFunc);
	sf::Text settingsText = sf::Text();
	settingsButton.createInteractable(Textures::HUDTextures.getSettingsButton(), settingsText);


	/// Left side of the HUD

	addTextDisplayBox(sf::Text("Firing Side: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesLeftSide, infoBoxScale);
	addTextDisplayBox(sf::Text("Manual Aim: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesLeftSide, infoBoxScale);
	addTextDisplayBox(sf::Text("Ship Type: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesLeftSide, infoBoxScale);
	addTextDisplayBox(sf::Text("Anchor: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesLeftSide, infoBoxScale);

	/// Right side of the HUD

	addTextDisplayBox(sf::Text("Gold: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesRightSide, infoBoxScale);
	addTextDisplayBox(sf::Text("Coords: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesRightSide, infoBoxScale);
	addTextDisplayBox(sf::Text("Velocity: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesRightSide, infoBoxScale);
	addTextDisplayBox(sf::Text("Speed: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesRightSide, infoBoxScale);
	addTextDisplayBox(sf::Text("Wind Direction: ", font, interactableTextSize), Textures::HUDTextures.getInfoBox(), shipPropertiesRightSide, infoBoxScale);
}

// This function will update the strings of the ship properties
void InGameHUD::updateShipPropertiesStrings() {
	/// Left side of the HUD

	// Update the firing side indicator
	std::string firingSide = "Firing Side: " + std::string(ship->getCannonHandler()->getFiringSide() == FiringSide::Port ? "Port" : "Starboard");
	shipPropertiesLeftSide[0].getText().setString(firingSide);

	// Update the manual aim indicator
	std::string manualAim = "Manual Aim: " + std::string(ship->getCannonHandler()->getAimTowardsMouse() ? "On" : "Off");
	shipPropertiesLeftSide[1].getText().setString(manualAim + " [" + Globals::keyToString(ship->getInputHandler()->getManualAimKey()) + "]");
	 
	// Update the ship type indicator
	std::string shipClass = "Ship Class: ";
	shipPropertiesLeftSide[2].getText().setString(shipClass + ship->getShipClassString());

	// Update the anchor drop indicator
	std::string anchorDrop = "Anchor: " + std::string(ship->getMovementHandler()->getDroppedAnchor() ? "Down" : "Up");
	shipPropertiesLeftSide[3].getText().setString(anchorDrop + " [" + Globals::keyToString(ship->getInputHandler()->getAnchorDropKey()) + "]");

	/// Right side of the HUD

	// Update the gold display
	std::string gold = "Gold: " + std::to_string(static_cast<int>(ship->getInventoryHandler()->getGold()));
	shipPropertiesRightSide[0].getText().setString(gold);

	// Update the ship coordinates display
	std::string coords = "Coords: " + std::to_string(static_cast<int>(ship->getSprite().getPosition().x)) + ", " + std::to_string(static_cast<int>(ship->getSprite().getPosition().y));
	shipPropertiesRightSide[1].getText().setString(coords);

	// Update the ship velocity display
	std::string velocity = "Velocity: " + std::to_string(static_cast<int>(ship->getMovementHandler()->getVelocity().x)) + ", " + std::to_string(static_cast<int>(ship->getMovementHandler()->getVelocity().y));
	shipPropertiesRightSide[2].getText().setString(velocity);

	// Update the ship speed display
	std::string speed = "Speed: " + std::to_string(static_cast<int>(ship->getMovementHandler()->getSpeed()));
	shipPropertiesRightSide[3].getText().setString(speed);

	// Update the wind direction display
	std::string windDirection = "Wind Direction: " + WH.getWindDirectionString();
	shipPropertiesRightSide[4].getText().setString(windDirection);
}
 
void InGameHUD::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();
	HUDView.setRotation(0.f);

	healthFraction = ship->getHealth() / ship->getSpecificShipProperties().maxHealth;

	// Set the health bar green to be proportional to ship health
	healthBarGreenSprite.setScale(healthBarScale.x * healthFraction, healthBarScale.y);

	// Set the position of the health bar to be in the center of the screen the top
	healthBarRedSprite.setPosition(HUDView.getCenter().x - healthBarRedSprite.getGlobalBounds().getSize().x / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding);
	healthBarGreenSprite.setPosition(healthBarRedSprite.getPosition());

	float healthTextX = healthBarRedSprite.getPosition().x + healthBarRedSprite.getLocalBounds().getSize().x / 2 - healthText.getGlobalBounds().getSize().x / 2;
	float healthTextY = healthBarRedSprite.getPosition().y + healthBarRedSprite.getLocalBounds().getSize().y / 2 - healthText.getGlobalBounds().getSize().y / 2;
	healthText.setPosition(healthTextX, healthTextY);
	healthText.setString("Health: " + std::to_string(ship->getHealth()));

	// Set the settings button to be in the top left corner
	settingsButton.setPosition(sf::Vector2f(HUDView.getCenter().x - window->getSize().x / 2u + padding, healthBarGreenSprite.getPosition().y));

	// Set the mini map to be in the top right corner
	minimap.setMinimapPosition(sf::Vector2f(HUDView.getCenter().x + window->getSize().x / 2u - minimap.getMinimapSprite().getGlobalBounds().height - padding, healthBarGreenSprite.getPosition().y));

	// Set the position of the info boxes to be below the settings button on the left side
	for (int i = 0; i < shipPropertiesLeftSide.size(); i++) {
		float x = HUDView.getCenter().x - window->getSize().x / 2u + padding;
		float y = settingsButton.getSprite().getPosition().y + settingsButton.getSprite().getGlobalBounds().getSize().y + padding + i * (shipPropertiesLeftSide[i].getSprite().getGlobalBounds().getSize().y + padding);
		shipPropertiesLeftSide[i].setPosition(sf::Vector2f(x, y));
	}

	// Set the position of the info boxes to be below the mini map on the right side
	for (int i = 0; i < shipPropertiesRightSide.size(); i++) {
		float x = HUDView.getCenter().x + window->getSize().x / 2u - shipPropertiesRightSide[i].getSprite().getGlobalBounds().getSize().x - padding;
		float y = minimap.getMinimapSprite().getPosition().y + 2 * minimap.getMinimapSprite().getGlobalBounds().height / 2.f + padding + shipPropertiesRightSide[i].getSprite().getGlobalBounds().getSize().y / 2 + i * (shipPropertiesRightSide[i].getSprite().getGlobalBounds().getSize().y + padding);
		shipPropertiesRightSide[i].setPosition(sf::Vector2f(x, y));
	}

	// Set the position of the wind vector to be in the bottom right corner of the screen
	sf::Vector2f windVectorPosition = sf::Vector2f(HUDView.getCenter().x + window->getSize().x / 2u - padding - 200.f, HUDView.getCenter().y + window->getSize().y / 2u - padding - 200.f);
	windVector = WH.getWindDirectionIndicator(windVectorPosition, 5.f);
	windCircle.setPosition(windVectorPosition - sf::Vector2f(windCircle.getRadius(), windCircle.getRadius()));
	windCircle.setRadius(5.f);
	windText = sf::Text("Wind Speed: " + std::to_string(static_cast<int>(WH.getWindSpeed())), font, 20);
	windText.setPosition(windVectorPosition.x - windText.getGlobalBounds().getSize().x / 2, windVectorPosition.y - windText.getGlobalBounds().getSize().y - padding);
	windText.setFillColor(sf::Color::Black);
}

void InGameHUD::interactWithMenuItems() {
	// Interact with the settings button
	settingsButton.interact(window,  context.GSM.get());
}

void InGameHUD::update() {
	minimap.update();
	updateShipPropertiesStrings();
	setInteractablePositions();
	interactWithMenuItems();
}

void InGameHUD::draw() {
	// Draw the health bar
	window->draw(healthBarRedSprite);
	window->draw(healthBarGreenSprite);
	window->draw(healthText);

	minimap.draw(window);

	for (auto& property : shipPropertiesLeftSide) {
		property.draw(window);
	}

	for (auto& property : shipPropertiesRightSide) {
		property.draw(window);
	}

	// Draw the interactables
	settingsButton.draw(window);

	// Draw a vector for the wind speed and direction in the bottom right corner of the screen
	window->draw(windVector);
	window->draw(windCircle);
	window->draw(windText);													
}