#include "InGameHUD_PG.h"

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
	// Grab the global game state manager
	GSM = &GlobalGameStateManager::getInstance();

	// Add a settings button
	std::function<void()> settingsFunc = [this]() { GSM->changeGameState(GameState::OptionsMenu); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc);
	sf::Text settingsText = sf::Text();
	settingsButton->createInteractable(GlobalTextureHandler::getInstance().getHUDTextures().getSettingsButton(), settingsText);
	addInteractableToMenu(std::move(settingsButton));


	/// Left side of the HUD

	addInfoBox("Firing Side: ", shipPropertiesLeftSide);
	addInfoBox("Manual Aim: ", shipPropertiesLeftSide);
	addInfoBox("Ship Type: ", shipPropertiesLeftSide);
	addInfoBox("Anchor: ", shipPropertiesLeftSide);

	/// Right side of the HUD

	addInfoBox("Gold: ", shipPropertiesRightSide);
	addInfoBox("Coords: ", shipPropertiesRightSide);
	addInfoBox("Velocity: ", shipPropertiesRightSide);
	addInfoBox("Speed: ", shipPropertiesRightSide);
	addInfoBox("Wind Direction: ", shipPropertiesRightSide);
}

// General function to add an info box with a string to the HUD
void InGameHUD::addInfoBox(std::string text, std::vector<std::unique_ptr<TextDisplayBox>>& destination) const {
	std::unique_ptr<TextDisplayBox> infoBox = std::make_unique<TextDisplayBox>();
	sf::Text infoText = sf::Text(text, font, interactableTextSize);
	infoBox->createInteractable(GlobalTextureHandler::getInstance().getHUDTextures().getInfoBox(), infoText, infoBoxScale);
	destination.push_back(std::move(infoBox));
}

// This function will update the strings of the ship properties
void InGameHUD::updateShipPropertiesString() {
	/// Left side of the HUD

	// Update the firing side indicator
	std::string firingSide = "Firing Side: " + std::string(ship->getCannonHandler().getFiringSide() == FiringSide::Port ? "Port" : "Starboard");
	shipPropertiesLeftSide[0]->getText().setString(firingSide);

	// Update the manual aim indicator
	std::string manualAim = "Manual Aim: " + std::string(ship->getCannonHandler().getAimTowardsMouse() ? "On" : "Off");
	shipPropertiesLeftSide[1]->getText().setString(manualAim + " [" + ship->getInputHandler().getManualAimKeyString() + "]");

	// Update the ship type indicator
	std::string shipClass = "Ship Class: ";
	shipPropertiesLeftSide[2]->getText().setString(shipClass + ship->getShipClassString());

	// Update the anchor drop indicator
	std::string anchorDrop = "Anchor: " + std::string(ship->getMovementHandler().getDroppedAnchor() ? "Down" : "Up");
	shipPropertiesLeftSide[3]->getText().setString(anchorDrop + " [" + ship->getInputHandler().getAnchorDropKeyString() + "]");

	/// Right side of the HUD

	// Update the gold display
	std::string gold = "Gold: " + std::to_string(static_cast<int>(ship->getInventoryHandler().getGold()));
	shipPropertiesRightSide[0]->getText().setString(gold);

	// Update the ship coordinates display
	std::string coords = "Coords: " + std::to_string(static_cast<int>(ship->getSprite().getPosition().x)) + ", " + std::to_string(static_cast<int>(ship->getSprite().getPosition().y));
	shipPropertiesRightSide[1]->getText().setString(coords);

	// Update the ship velocity display
	std::string velocity = "Velocity: " + std::to_string(static_cast<int>(ship->getMovementHandler().getVelocity().x)) + ", " + std::to_string(static_cast<int>(ship->getMovementHandler().getVelocity().y));
	shipPropertiesRightSide[2]->getText().setString(velocity);

	// Update the ship speed display
	std::string speed = "Speed: " + std::to_string(static_cast<int>(ship->getMovementHandler().getSpeed()));
	shipPropertiesRightSide[3]->getText().setString(speed);

	// Update the wind direction display
	std::string windDirection = "Wind Direction: " + GlobalWindController::getInstance().getWindDirectionString();
	shipPropertiesRightSide[4]->getText().setString(windDirection);
}
 
void InGameHUD::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();
	HUDView.setRotation(0.f);

	healthFraction = 100 * ship->getHealth() / ship->getShipProperties().maxHealth;

	// Set the health bar green to be proportional to ship health
	healthBarGreenSprite.setScale(healthBarScale.x * healthFraction / 100, healthBarScale.y);

	// Set the position of the health bar to be in the center of the screen the top
	healthBarRedSprite.setPosition(HUDView.getCenter().x - healthBarRedSprite.getGlobalBounds().getSize().x / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding);
	healthBarGreenSprite.setPosition(healthBarRedSprite.getPosition());

	float healthTextX = healthBarRedSprite.getPosition().x + healthBarRedSprite.getLocalBounds().getSize().x / 2 - healthText.getGlobalBounds().getSize().x / 2;
	float healthTextY = healthBarRedSprite.getPosition().y + healthBarRedSprite.getLocalBounds().getSize().y / 2 - healthText.getGlobalBounds().getSize().y / 2;
	healthText.setPosition(healthTextX, healthTextY);
	healthText.setString("Health: " + std::to_string(static_cast<int>(healthFraction)));

	// Set the settings button to be in the top left corner
	interactables[0]->setPosition(sf::Vector2f(HUDView.getCenter().x - window->getSize().x / 2u + padding, healthBarGreenSprite.getPosition().y));

	// Set the mini map to be in the top right corner
	minimap.setMinimapPosition(sf::Vector2f(HUDView.getCenter().x + window->getSize().x / 2u - minimap.getMinimapRadius() * 2 - padding, healthBarGreenSprite.getPosition().y));

	// Set the position of the info boxes to be below the settings button on the left side
	for (int i = 0; i < shipPropertiesLeftSide.size(); i++) {
		float x = HUDView.getCenter().x - window->getSize().x / 2u + padding;
		float y = interactables[0]->getSprite().getPosition().y + interactables[0]->getSprite().getGlobalBounds().getSize().y + padding + i * (shipPropertiesLeftSide[i]->getSprite().getGlobalBounds().getSize().y + padding);
		shipPropertiesLeftSide[i]->setPosition(sf::Vector2f(x, y));
	}

	// Set the position of the info boxes to be below the mini map on the right side
	for (int i = 0; i < shipPropertiesRightSide.size(); i++) {
		float x = HUDView.getCenter().x + window->getSize().x / 2u - shipPropertiesRightSide[i]->getSprite().getGlobalBounds().getSize().x - padding;
		float y = minimap.getMinimapPosition().y + 2 * minimap.getMinimapRadius() + padding + shipPropertiesRightSide[i]->getSprite().getGlobalBounds().getSize().y / 2 + i * (shipPropertiesRightSide[i]->getSprite().getGlobalBounds().getSize().y + padding);
		shipPropertiesRightSide[i]->setPosition(sf::Vector2f(x, y));
	}

	// Set the position of the wind vector to be in the bottom right corner of the screen
	sf::Vector2f windVectorPosition = sf::Vector2f(HUDView.getCenter().x + window->getSize().x / 2u - padding - 200.f, HUDView.getCenter().y + window->getSize().y / 2u - padding - 200.f);
	windVector = GlobalWindController::getInstance().getWindDirectionIndicator(windVectorPosition, 5.f);
	windCircle.setPosition(windVectorPosition - sf::Vector2f(windCircle.getRadius(), windCircle.getRadius()));
	windCircle.setRadius(5.f);
	windText = sf::Text("Wind Speed: " + std::to_string(static_cast<int>(GlobalWindController::getInstance().getWindSpeed())), font, 20);
	windText.setPosition(windVectorPosition.x - windText.getGlobalBounds().getSize().x / 2, windVectorPosition.y - windText.getGlobalBounds().getSize().y - padding);
	windText.setFillColor(sf::Color::Black);
}

void InGameHUD::draw() {
	// Draw the health bar
	window->draw(healthBarRedSprite);
	window->draw(healthBarGreenSprite);
	window->draw(healthText);

	minimap.draw();
	minimap.update();

	// Add and draw the ship properties
	updateShipPropertiesString();

	for (auto& property : shipPropertiesLeftSide) {
		property->draw();
	}

	for (auto& property : shipPropertiesRightSide) {
		property->draw();
	}

	// Draw the interactables
	for (auto& interactable : interactables) {
		interactable->draw();
	}

	// Draw a vector for the wind speed and direction in the bottom right corner of the screen
	window->draw(windVector);
	window->draw(windCircle);
	window->draw(windText);

	// Interact
	setInteractablePositions();
	interactWithMenuItems();															
}