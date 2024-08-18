#include "InventoryMenu_PG.h"

using namespace PirateGame;

void InventoryMenu::setUpMenu() {
	menu.setTexture(context.GTH->getInventoryTextures().getBackground());

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
	shipDisplaySprite.setTexture(context.GTH->getShipTextures().getShipTextureManager().getTexture(ship->getShipClass()));
	shipDisplayBackground.setTexture(context.GTH->getInventoryTextures().getInventoryShipDisplay());

	// Prepare the inventory display background
	inventoryDisplayBackground.setTexture(context.GTH->getInventoryTextures().getInventoryDisplay());

	// Set up the scroll bar
	scrollBar.setUpScrollBar(scrollBarPosition, inventoryDisplayBackground.getGlobalBounds().getSize().y, inventoryPosition, inventoryDisplayBackground.getGlobalBounds().getSize(), scale);

	// Add the interactables to the menu and set their positions
	addInteractablesToMenu();
}

void InventoryMenu::addInteractablesToMenu() {
	inventoryBoxes.clear();
	this->inventory = ship->getInventoryHandler()->getInventory();
	for (auto& item : inventory){
		std::shared_ptr<TextDisplayBox> textDisplayBox = std::make_shared<TextDisplayBox>();
		sf::Text text = sf::Text(item.name + ": " + std::to_string(item.amount), font, interactableTextSize);
		textDisplayBox->createInteractable(context.GTH->getInventoryTextures().getInventoryItemDisplay(), text, scale);
		textDisplayBox->getText().setFillColor(sf::Color::Black);
		inventoryBoxes.push_back(textDisplayBox);
	}
	scrollBar.setInteractables(inventoryBoxes);

	shipDisplayInfo.clear();
	addTextDisplayBox(sf::Text(
		ship->getShipClassString() + "\n" +
		"Level: " + std::to_string(ship->getplayerLevel()) + "\n" +
		"Health: " + floatToString(ship->getHealth()) + " / " + floatToString(ship->getSpecificShipProperties().maxHealth) + "\n"
		"Speed: " + floatToString(ship->getSpecificShipProperties().baseSpeed) + "\n"
		"Cannons: " + std::to_string(ship->getSpecificShipProperties().numCannons) + "\n", 
		font, interactableTextSize), context.GTH->getInventoryTextures().getInventoryTextDisplay(), shipDisplayInfo, scale);
	shipDisplayInfo[0].getText().setFillColor(sf::Color::Black);
}

void InventoryMenu::setInteractablePositions() {
	// Set the position of the menu and title text
	menu.setPosition(window->getView().getCenter() - sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
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

}

void InventoryMenu::interactWithMenuItems() {
	/* nothing here yet */
}

void InventoryMenu::update() {
	if (inventory != ship->getInventoryHandler()->getInventory()) {
		addInteractablesToMenu();
	}
	setInteractablePositions();
	interactWithMenuItems();
	scrollBar.update(menu.getPosition());
}

void InventoryMenu::draw() {
	window->draw(menu);
	window->draw(titleText);
	window->draw(shipDisplayBackground);
	window->draw(shipDisplaySprite);
	for (auto& box : shipDisplayInfo) {
		box.draw(window);
	}
	for (auto& box : inventoryBoxes) {
		box->draw(window);
	}
	scrollBar.draw();
}