#include "InGameHUD_PG.h"

using namespace PirateGame;

void InGameHUD::setUpMenu() {
	interactableSize = sf::Vector2f(200, 50);

	healthText = sf::Text("Health: " + std::to_string(static_cast<int>(healthFraction)), font, 20);
	healthText.setFillColor(sf::Color::Black);

	// Set up the minimap
	minimap.setMinimapRadius(minimapSize);

	addInteractablesToMenu();
}

void InGameHUD::addInteractablesToMenu() {
	// Grab the global game state manager
	GSM = &GlobalValues::getInstance().getGSM();

	// Add a settings button
	std::function<void()> settingsFunc = [this]() { GSM->changeGameState(GameState::OptionsMenu); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc);
	settingsButton->createInteractable(interactableSize);
	settingsButton->setString("Settings");
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
void InGameHUD::addInfoBox(std::string text, std::vector<std::unique_ptr<TextDisplayBox>>& destination) {
	std::unique_ptr<TextDisplayBox> infoBox = std::make_unique<TextDisplayBox>();
	infoBox->createInteractable(interactableSize);
	infoBox->setString(text);
	destination.push_back(std::move(infoBox));
}

// This function will update the strings of the ship properties
void InGameHUD::updateShipPropertiesString() {
	/// Left side of the HUD

	// Update the firing side indicator
	std::string firingSide = "Firing Side: ";
	if (ship->getCannonHandler().getFiringSide() == FiringSide::Port) {
		firingSide += "Left";
		shipPropertiesLeftSide[0]->setString(firingSide + " [" + ship->getInputHandler().getPortMouseButtonString() + "]");
	}
	else {
		firingSide += "Right";
		shipPropertiesLeftSide[0]->setString(firingSide + " [" + ship->getInputHandler().getStarboardMouseButtonString() + "]");
	}

	// Update the manual aim indicator
	std::string manualAim = "Manual Aim: ";
	if (ship->getCannonHandler().getAimTowardsMouse()) {
		manualAim += "On";
	}
	else {
		manualAim += "Off";
	} 
	shipPropertiesLeftSide[1]->setString(manualAim + " [" + ship->getInputHandler().getManualAimKeyString() + "]");

	// Update the ship type indicator
	std::string shipClass = "Ship Class: ";
	shipPropertiesLeftSide[2]->setString(shipClass + ship->getShipClassString());

	// Update the anchor drop indicator
	std::string anchorDrop = "Anchor: ";
	if (ship->getMovementHandler().getDroppedAnchor()) {
		anchorDrop += "Down";
	}
	else {
		anchorDrop += "Up";
	}
	shipPropertiesLeftSide[3]->setString(anchorDrop + " [" + ship->getInputHandler().getAnchorDropKeyString() + "]");

	/// Right side of the HUD

	// Update the gold display
	std::string gold = "Gold: " + std::to_string(static_cast<int>(ship->getInventoryHandler().getGold()));
	shipPropertiesRightSide[0]->setString(gold);

	// Update the ship coordinates display
	std::string coords = "Coords: " + std::to_string(static_cast<int>(ship->getSprite().getPosition().x)) + ", " + std::to_string(static_cast<int>(ship->getSprite().getPosition().y));
	shipPropertiesRightSide[1]->setString(coords);

	// Update the ship velocity display
	std::string velocity = "Velocity: " + std::to_string(static_cast<int>(ship->getMovementHandler().getVelocity().x)) + ", " + std::to_string(static_cast<int>(ship->getMovementHandler().getVelocity().y));
	shipPropertiesRightSide[2]->setString(velocity);

	// Update the ship speed display
	std::string speed = "Speed: " + std::to_string(static_cast<int>(ship->getMovementHandler().getSpeed()));
	shipPropertiesRightSide[3]->setString(speed);

	// Update the wind direction display
	std::string windDirection = "Wind Direction: " + GlobalValues::getInstance().getWindController().getWindDirectionString();
	shipPropertiesRightSide[4]->setString(windDirection);
}
 
void InGameHUD::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();
	HUDView.setRotation(0);

	healthFraction = 100 * ship->getHealth() / ship->getShipProperties().maxHealth;

	// Set the health bar green to be proportional to ship health
	healthBarGreenSprite.setScale(healthBarScale.x * healthFraction / 100, healthBarScale.y);

	// Set the position of the health bar to be in the center of the screen the top
	healthBarRedSprite.setPosition(HUDView.getCenter().x - healthBarRedSprite.getGlobalBounds().width / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding);
	healthBarGreenSprite.setPosition(healthBarRedSprite.getPosition());

	float healthTextX = healthBarGreenSprite.getPosition().x + healthBarRedSprite.getLocalBounds().width / 2 - healthText.getGlobalBounds().width / 2;
	float healthTextY = healthBarGreenSprite.getPosition().y + healthBarRedSprite.getLocalBounds().height / 2 - healthText.getGlobalBounds().height / 2;
	healthText.setPosition(healthTextX, healthTextY);
	healthText.setString("Health: " + std::to_string(static_cast<int>(healthFraction)));

	// Set the settings button to be in the top left corner
	interactables[0]->setPosition(sf::Vector2f(HUDView.getCenter().x - window->getSize().x / 2u + interactables[0]->getSprite().getGlobalBounds().width / 2, 
		healthBarGreenSprite.getPosition().y + interactables[0]->getSprite().getGlobalBounds().height / 2));

	// Set the mini map to be in the top right corner
	minimap.setMinimapPosition(sf::Vector2f(HUDView.getCenter().x + window->getSize().x / 2u - minimap.getMinimapRadius() * 2 - padding, healthBarGreenSprite.getPosition().y));

	// Set the position of the info boxes to be below the settings button on the left side
	for (int i = 0; i < shipPropertiesLeftSide.size(); i++) {
		float x = HUDView.getCenter().x - window->getSize().x / 2u + shipPropertiesLeftSide[i]->getSprite().getGlobalBounds().width / 2;
		float y = interactables[0]->getPosition().y + interactables[0]->getSprite().getGlobalBounds().height + padding + i * (shipPropertiesLeftSide[i]->getSprite().getGlobalBounds().height + padding);
		shipPropertiesLeftSide[i]->setPosition(sf::Vector2f(x, y));
	}

	// Set the position of the info boxes to be below the mini map on the right side
	for (int i = 0; i < shipPropertiesRightSide.size(); i++) {
		float x = HUDView.getCenter().x + window->getSize().x / 2u - shipPropertiesRightSide[i]->getSprite().getGlobalBounds().width / 2;
		float y = minimap.getMinimapPosition().y + 2 * minimap.getMinimapRadius() + padding + shipPropertiesRightSide[i]->getSprite().getGlobalBounds().height / 2 + i * (shipPropertiesRightSide[i]->getSprite().getGlobalBounds().height + padding);
		shipPropertiesRightSide[i]->setPosition(sf::Vector2f(x, y));
	}

	// Set the position of the wind vector to be in the bottom right corner of the screen
	sf::Vector2f windVectorPosition = sf::Vector2f(HUDView.getCenter().x + window->getSize().x / 2u - padding - 200.f, HUDView.getCenter().y + window->getSize().y / 2u - padding - 200.f);
	windVector = GlobalValues::getInstance().getWindController().getWindDirectionIndicator(windVectorPosition, 5.f);
	windCircle.setPosition(windVectorPosition - sf::Vector2f(windCircle.getRadius(), windCircle.getRadius()));
	windCircle.setRadius(5.f);
	windText = sf::Text("Wind Speed: " + std::to_string(static_cast<int>(GlobalValues::getInstance().getWindController().getWindSpeed())), font, 20);
	windText.setPosition(windVectorPosition.x - windText.getGlobalBounds().width / 2, windVectorPosition.y - windText.getGlobalBounds().height - padding);
	windText.setFillColor(sf::Color::Black);
}

void InGameHUD::draw() {
	// Draw the health bar
	window->draw(healthBarRedSprite);
	window->draw(healthBarGreenSprite);
	window->draw(healthText);

	minimap.draw();
	minimap.update(ship->getSprite().getRotation());

	// Add and draw the ship properties
	updateShipPropertiesString();

	for (auto& property : shipPropertiesLeftSide) {
		property->getFrame().setFillColor(sf::Color::Transparent);
		property->getFrame().setOutlineColor(sf::Color::Transparent);
		property->draw();
	}

	for (auto& property : shipPropertiesRightSide) {
		property->getFrame().setFillColor(sf::Color::Transparent);
		property->getFrame().setOutlineColor(sf::Color::Transparent);
		property->draw();
	}

	// Draw the interactables
	for (auto& interactable : interactables) {
		//interactable->getBackground().setFillColor(sf::Color::Transparent);
		interactable->getFrame().setFillColor(sf::Color::Transparent);
		interactable->getFrame().setOutlineThickness(0.f);
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