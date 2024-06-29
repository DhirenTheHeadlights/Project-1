#include "InventoryMenu_PG.h"

using namespace PirateGame;

void InventoryMenu::setUpMenu() {
	//Load in Player ship and inventory
	this->inventory = ship->getInventoryHandler()->getInventory();

	//TEMPORARY: MENU TEXTURES REQUIRED (20% window width, 100% height)
	menu.setTexture(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getBackground());
	

	interactableTextSize = 30;

	// Create Pop-up frame
	float menuYScale = yProportion * (window->getSize().y / menu.getGlobalBounds().getSize().y);
	float menuXScale = xProportion * (window->getSize().x / menu.getGlobalBounds().getSize().x);
	menu.setScale(menuXScale, menuYScale);



	// Set up the title text
	titleText = sf::Text("Inventory", font, textSize);
	titleText.setFillColor(sf::Color::White);


	// Prepare the ship display
	//TEMPORARY: SHIP DISPLAY SPRITE REQUIRED
	shipDisplaySprite.setTexture(GlobalTextureHandler::getInstance().getShipTextures().getShipTextureManager().getTexture(ship->getShipClass()));

	// Add the interactables to the menu and set their positions
	addInteractablesToMenu();
	setInteractablePositions();
}

void InventoryMenu::addInteractablesToMenu() {
	for (auto& item : inventory) {
		addTextDisplayBox(sf::Text(item.name + ": " + std::to_string(item.amount), font, interactableTextSize), GlobalTextureHandler::getInstance().getInteractableTextures().getBlackGrayButton(), inventoryBoxes);
	}
	addTextDisplayBox(sf::Text(
		ship->getShipClassString() + "\n" +
		"Level: " + std::to_string(ship->getplayerLevel()) + "\n" +
		"Health: " + floatToString(ship->getHealth()) + " / " + floatToString(ship->getSpecificShipProperties().maxHealth) + "\n"
		"Speed: " + floatToString(ship->getSpecificShipProperties().baseSpeed) + "\n"
		"Cannons: " + std::to_string(ship->getSpecificShipProperties().numCannons) + "\n"
		, font, interactableTextSize), GlobalTextureHandler::getInstance().getInteractableTextures().getBlackGrayButton(), shipDisplayInfo);

}

void InventoryMenu::setInteractablePositions() {
		// Set the position of the ship display
	shipDisplaySprite.setPosition(menu.getPosition().x + 0.1 * menu.getGlobalBounds().getSize().x, menu.getPosition().y + 0.1 * menu.getGlobalBounds().getSize().y);

	// Set the position of the ship display info
	shipDisplayInfo[0].setPosition(sf::Vector2f(shipDisplaySprite.getPosition().x + shipDisplaySprite.getGlobalBounds().getSize().x + padding, 
												shipDisplaySprite.getPosition().y + shipDisplaySprite.getGlobalBounds().getSize().y + padding));

	// Set the position of the inventory boxes
	sf::Vector2f boxPositionIterator = sf::Vector2f(menu.getPosition().x + 0.5 * menu.getGlobalBounds().getSize().x,
									menu.getPosition().y + menu.getGlobalBounds().getSize().y);
	for (size_t i = 0; i < inventoryBoxes.size(); i++) {
		inventoryBoxes[i].setPosition(boxPositionIterator);
		boxPositionIterator.y += inventoryBoxes[i].getSprite().getGlobalBounds().getSize().y + padding;
	}


}
void InventoryMenu::update() {
	menu.setPosition(static_cast<float>(window->getView().getCenter().x - window->getSize().x / 2u),
		window->getView().getCenter().y - static_cast<float>(window->getSize().y / 2u));
	titleText.setPosition(menu.getPosition().x + menu.getGlobalBounds().getSize().x / 2 - titleText.getGlobalBounds().getSize().x / 2,
		menu.getPosition().y - titleText.getGlobalBounds().getSize().y - padding);
	setInteractablePositions();
}

void InventoryMenu::draw() {
	window->draw(menu);
	window->draw(titleText);
	window->draw(shipDisplaySprite);
	for (auto& box : shipDisplayInfo) {
		window->draw(box.getSprite());
		window->draw(box.getText());
	}
	for (auto& box : inventoryBoxes) {
		window->draw(box.getSprite());
		window->draw(box.getText());
	}
}