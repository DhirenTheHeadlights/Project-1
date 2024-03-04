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

	addInteractablesToMenu();
}

void InGameHUD::addInteractablesToMenu() {
	// Grab the global game state manager
	GSM = &GlobalValues::getInstance().getGSM();

	// Add a settings button
	std::function<void()> settingsFunc = [this]() { GSM->changeGameState(GameState::OptionsMenu); };
	std::unique_ptr<Button> settingsButton = std::make_unique<Button>(settingsFunc);
	settingsButton->createInteractable(sf::Vector2f(200, 50));
	settingsButton->setString("Settings");
	addInteractableToMenu(std::move(settingsButton));

	// Add information boxes for the ship state and properties
	std::unique_ptr<TextDisplayBox> shipFiringSideIndicator = std::make_unique<TextDisplayBox>();
	shipFiringSideIndicator->createInteractable(sf::Vector2f(200, 50));
	std::string firingSide = "Firing Side: ";
	shipFiringSideIndicator->setString(firingSide);
	shipProperties.push_back(std::move(shipFiringSideIndicator));

	std::unique_ptr<TextDisplayBox> manualAimIndicator = std::make_unique<TextDisplayBox>();
	manualAimIndicator->createInteractable(sf::Vector2f(200, 50));
	std::string manualAim = "Manual Aim: ";
	manualAimIndicator->setString(manualAim);
	shipProperties.push_back(std::move(manualAimIndicator));
}

// This function will update the strings of the ship properties
void InGameHUD::updateShipPropertiesString() {
	// Update the firing side indicator
	std::string firingSide = "Firing Side: ";
	if (ship->getCannonHandler().getFiringSide() == FiringSide::Port) {
		firingSide += "Left";
		shipProperties[0]->setString(firingSide + " [" + ship->getInputHandler().getPortMouseButtonString() + "]");
	}
	else {
		firingSide += "Right";
		shipProperties[0]->setString(firingSide + " [" + ship->getInputHandler().getStarboardMouseButtonString() + "]");
	}

	// Update the manual aim indicator
	std::string manualAim = "Manual Aim: ";
	if (ship->getCannonHandler().getAimTowardsMouse()) {
		manualAim += "On";
	}
	else {
		manualAim += "Off";
	} 
	shipProperties[1]->setString(manualAim + " [" + ship->getInputHandler().getManualAimKeyString() + "]");
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
	minimap.setPosition(HUDView.getCenter().x + healthBarSize.x / 2, HUDView.getCenter().y - window->getSize().y / 2 + padding);

	// Set the position of the info boxes to be below the settings button on the left side
	for (int i = 0; i < shipProperties.size(); i++) {
		float x = HUDView.getCenter().x - window->getSize().x / 2u + padding;
		float y = interactables[0]->getPosition().y + interactables[0]->getBackground().getSize().y + padding + i * (shipProperties[i]->getBackground().getSize().y + padding);
		shipProperties[i]->setPosition(sf::Vector2f(x, y));
	}
}

void InGameHUD::draw() {
	// Draw the health bar
	healthBarRed.setPosition(healthBarGreen.getPosition());
	window->draw(healthBarRed);
	window->draw(healthBarGreen);
	window->draw(healthText);

	window->draw(minimap);

	// Add and draw the ship properties
	updateShipPropertiesString();
	for (auto& property : shipProperties) {
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