#include "InventoryMenu_PG.h"

using namespace PirateGame;

void InventoryMenu::setUpMenu() {
	// Load in Player ship and inventory
	this->inventory = ship->getInventoryHandler()->getInventory();

	// TEMPORARY: MENU TEXTURES REQUIRED (20% window width, 100% height)
	menu.setTexture(GlobalTextureHandler::getInstance().getInventoryTextures().getBackground());

	interactableTextSize = 25;

	// Create Pop-up frame and set scales
	scale = sf::Vector2f(xProportion * window->getSize().x / menu.getGlobalBounds().getSize().x, yProportion * window->getSize().y / menu.getGlobalBounds().getSize().y);
	menu.setScale(scale);
	shipDisplayBackground.setScale(scale);
	inventoryDisplayBackground.setScale(scale);

	shipDisplayPosition = sf::Vector2f(shipDisplayPosition.x * scale.x, shipDisplayPosition.y * scale.y);
	shipInfoPosition = sf::Vector2f(shipInfoPosition.x * scale.x, shipInfoPosition.y * scale.y);
	inventoryPosition = sf::Vector2f(inventoryPosition.x * scale.x, inventoryPosition.y * scale.y);
	scrollBarPosition = sf::Vector2f(scrollBarPosition.x * scale.x, scrollBarPosition.y * scale.y);

	// Set up the title text
	titleText = sf::Text("Inventory", font, textSize);
	titleText.setFillColor(sf::Color::White);

	// Prepare the ship display
	shipDisplaySprite.setTexture(GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(ship->getShipClass()));
	shipDisplayBackground.setTexture(GlobalTextureHandler::getInstance().getInventoryTextures().getInventoryShipDisplay());

	// Prepare the inventory display background
	inventoryDisplayBackground.setTexture(GlobalTextureHandler::getInstance().getInventoryTextures().getInventoryDisplay());

	// Add the interactables to the menu and set their positions
	addInteractablesToMenu();
	setInteractablePositions();

	scrollBar.setUpScrollBar(scrollBarPosition, inventoryDisplayBackground.getGlobalBounds().getSize().y, inventoryPosition, inventoryDisplayBackground.getGlobalBounds().getSize().y);
}

void InventoryMenu::addInteractablesToMenu() {
	for (auto& item : inventory) {
		addTextDisplayBox(sf::Text(item.name + ": " + std::to_string(item.amount), font, interactableTextSize), GlobalTextureHandler::getInstance().getInventoryTextures().getInventoryItemDisplay(), inventoryBoxes, scale);
	}
	addTextDisplayBox(sf::Text(
		ship->getShipClassString() + "\n" +
		"Level: " + std::to_string(ship->getplayerLevel()) + "\n" +
		"Health: " + floatToString(ship->getHealth()) + " / " + floatToString(ship->getSpecificShipProperties().maxHealth) + "\n"
		"Speed: " + floatToString(ship->getSpecificShipProperties().baseSpeed) + "\n"
		"Cannons: " + std::to_string(ship->getSpecificShipProperties().numCannons) + "\n", 
		font, interactableTextSize), GlobalTextureHandler::getInstance().getInventoryTextures().getInventoryTextDisplay(), shipDisplayInfo, scale);
	scrollBar.setInteractables(inventoryBoxes);
}

void InventoryMenu::setInteractablePositions() {
	// Set the position of the menu and title text
	menu.setPosition(static_cast<float>(window->getView().getCenter().x - window->getSize().x / 2u),
		window->getView().getCenter().y - static_cast<float>(window->getSize().y / 2u));
	titleText.setPosition(menu.getPosition().x + menu.getGlobalBounds().getSize().x / 2 - titleText.getGlobalBounds().getSize().x / 2,
		menu.getPosition().y - titleText.getGlobalBounds().getSize().y - padding);

	// Set the position of the ship display
	shipDisplayBackground.setPosition(menu.getPosition() + shipDisplayPosition);
	shipDisplaySprite.setPosition(shipDisplayBackground.getPosition() + sf::Vector2f(0.5f * shipDisplayBackground.getGlobalBounds().getSize().x - 0.5f * shipDisplaySprite.getGlobalBounds().getSize().x,
		0.5f * shipDisplayBackground.getGlobalBounds().getSize().y - 0.5f * shipDisplaySprite.getGlobalBounds().getSize().y));

	// Set the position of the ship display info
	shipDisplayInfo[0].setPosition(menu.getPosition() + shipInfoPosition);

	// Set the position of the inventory display
	inventoryDisplayBackground.setPosition(menu.getPosition() + inventoryPosition);

	scrollBar.update(menu.getPosition());
}

void InventoryMenu::interactWithMenuItems() {
	/* nothing here yet */
}

void InventoryMenu::update() {
	setInteractablePositions();
	interactWithMenuItems();
}

void InventoryMenu::draw() {
	window->draw(menu);
	window->draw(titleText);
	window->draw(shipDisplayBackground);
	window->draw(shipDisplaySprite);
	for (auto& box : shipDisplayInfo) {
		box.draw();
	}
	for (auto& box : inventoryBoxes) {
		box->draw();
	}
	scrollBar.draw();
}