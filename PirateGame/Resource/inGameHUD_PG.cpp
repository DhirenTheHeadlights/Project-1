#include "InGameHUD_PG.h"

using namespace PirateGame;

void InGameHUD::setUpMenu() {
	healthBarSize = sf::Vector2f(window->getSize().x - 400 - padding * 2, 50);
	interactableSize = sf::Vector2f(200, 50);

	// Set up the health bar
	healthBarGreen.setSize(sf::Vector2f(healthBarSize));
	healthBarGreen.setFillColor(sf::Color::Green);
	healthBarRed.setSize(sf::Vector2f(healthBarSize));
	healthBarRed.setFillColor(sf::Color::Red);
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
	addInfoBox("Ship Class: ", shipPropertiesLeftSide);
	addInfoBox("Ship Type: ", shipPropertiesLeftSide);

	/// Right side of the HUD

	addInfoBox("Gold: ", shipPropertiesRightSide);
	addInfoBox("Coords: ", shipPropertiesRightSide);
	addInfoBox("Velocity: ", shipPropertiesRightSide);
	addInfoBox("Speed: ", shipPropertiesRightSide);
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

	// Update the ship class indicator
	std::string shipType = "Ship Type: ";
	shipPropertiesLeftSide[3]->setString(shipType + ship->getShipTypeString());

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

}
 
void InGameHUD::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();
	HUDView.setRotation(0);

	// Set the health bar red to be proportional to the health from the ship
	healthBarGreen.setSize(sf::Vector2f(healthBarSize.x - (healthBarSize.x * (1 - (ship->getHealth() / ship->getShipProperties().maxHealth))), healthBarSize.y));
	healthBarRed.setSize(healthBarSize);

	// Set the position of the health bar and text
	healthBarGreen.setPosition(HUDView.getCenter().x - healthBarSize.x / 2, HUDView.getCenter().y - window->getSize().y / 2u + padding);
	healthBarRed.setPosition(healthBarGreen.getPosition());

	float healthTextX = healthBarGreen.getPosition().x + healthBarGreen.getSize().x / 2 - healthText.getGlobalBounds().width / 2;
	float healthTextY = healthBarGreen.getPosition().y + healthBarGreen.getSize().y / 2 - healthText.getGlobalBounds().height / 2;
	healthText.setPosition(healthTextX, healthTextY);
	healthText.setString("Health: " + std::to_string(static_cast<int>(100 * ship->getHealth() / ship->getShipProperties().maxHealth)));

	// Set the settings button to be in the top left corner
	interactables[0]->setPosition(sf::Vector2f(HUDView.getCenter().x - window->getSize().x / 2u + padding, healthBarGreen.getPosition().y));

	// Set the mini map to be in the top right corner with padding right by the health bars
	minimap.setMinimapPosition(sf::Vector2f(HUDView.getCenter().x + healthBarSize.x / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding));

	// Set the position of the info boxes to be below the settings button on the left side
	for (int i = 0; i < shipPropertiesLeftSide.size(); i++) {
		float x = HUDView.getCenter().x - window->getSize().x / 2u + padding;
		float y = interactables[0]->getPosition().y + interactables[0]->getBackground().getSize().y + padding + i * (shipPropertiesLeftSide[i]->getBackground().getSize().y + padding);
		shipPropertiesLeftSide[i]->setPosition(sf::Vector2f(x, y));
	}

	// Set the position of the info boxes to be below the mini map on the right side
	for (int i = 0; i < shipPropertiesRightSide.size(); i++) {
		float x = HUDView.getCenter().x + healthBarSize.x / 2;
		float y = minimap.getMinimapPosition().y + 2 * minimap.getMinimapRadius() + padding + i * (shipPropertiesRightSide[i]->getBackground().getSize().y + padding);
		shipPropertiesRightSide[i]->setPosition(sf::Vector2f(x, y));
	}
}

void InGameHUD::draw() {
	// Draw the health bar
	healthBarRed.setPosition(healthBarGreen.getPosition());
	window->draw(healthBarRed);
	window->draw(healthBarGreen);
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

	// Interact
	setInteractablePositions();
	interactWithMenuItems();															
}