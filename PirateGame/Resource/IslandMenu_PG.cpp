#include "IslandMenu_PG.h"

using namespace PirateGame;

void IslandMenu::setUpMenu() {
	// Set up the inital menu that asks the player if they wish to enter the island
	menu.setSize(initalMenuSize);
	menu.setFillColor(sf::Color::White);
	menu.setOutlineColor(sf::Color::Black);
	menu.setOutlineThickness(2.0f);

	// Set up the island name
	std::vector<std::string> randomIslandNames = { "Tortuga", "Port Royal", "Nassau", "Barbados", "Jamaica", "St. Kitts", "St. Lucia", "St. Vincent", "St. Thomas", "St. Croix" };
	titleText = sf::Text("Island: " + randomIslandNames[rand() % randomIslandNames.size()], font, static_cast<unsigned int>(textSize));
	titleText.setFillColor(textColor);
	
	addInteractablesToMenu();
}

void IslandMenu::addInteractablesToMenu() {
	// Grab the global game state manager
	GSM = &GlobalValues::getInstance().getGSM();

	// Initial buttons to enter the island
	std::function<void()> enterIsland = [this]() {
		GSM->changeGameState(GameState::LandMassInteraction);
		enteredIsland = true;
		};
	std::unique_ptr<Button> enterIslandButton = std::make_unique<Button>(enterIsland);
	enterIslandButton->createInteractable(sf::Vector2f(200, 50));
	enterIslandButton->setString("Enter Island");
	addInteractableToMenu(std::move(enterIslandButton));

	std::function<void()> leaveIsland = [this]() {
		GSM->changeGameState(GameState::GameLoop);
		enteredIsland = false;
	};
	std::unique_ptr<Button> leaveIslandButton = std::make_unique<Button>(leaveIsland);
	leaveIslandButton->createInteractable(sf::Vector2f(200, 50));
	leaveIslandButton->setString("Leave Island");
	addInteractableToMenu(std::move(leaveIslandButton));

	// Add the market interactables
	addMarketInteractables();
}

void IslandMenu::addMarketInteractables() {
	// Add every item from the market to the menu along
	// with a buy and sell button and the price of the item
	for (auto& item : market) {
		// Create the buy button
		std::function<void()> buyItem = [this, &item]() {
			// Check if the player has enough gold
			if (ship->getInventoryHandler().getGold() >= item.price) {
				// Add the item to the inventory
				ship->getInventoryHandler().getInventory().push_back(item);
				// Remove the gold from the player
				ship->getInventoryHandler().removeGold(item.price);
			}
		};
		std::unique_ptr<Button> buyButton = std::make_unique<Button>(buyItem);
		buyButton->createInteractable(sf::Vector2f(50, 50));
		buyButton->setString("Buy");
		buyButtons.push_back(std::move(buyButton));

		// Create the sell button
		std::function<void()> sellItem = [this, &item]() {
			// Check if the player has the item
			if (ship->getInventoryHandler().getItem(item.name).amount > 0) {
				// Remove the item from the inventory
				ship->getInventoryHandler().removeItem(item);
				// Add the gold to the player
				ship->getInventoryHandler().addGold(item.price);
			}
		};
		std::unique_ptr<Button> sellButton = std::make_unique<Button>(sellItem);
		sellButton->createInteractable(sf::Vector2f(50, 50));
		sellButton->setString("Sell");
		sellButtons.push_back(std::move(sellButton));

		// Create the text display box for the item
		std::unique_ptr<TextDisplayBox> itemDisplay = std::make_unique<TextDisplayBox>();
		itemDisplay->createInteractable(sf::Vector2f(200, 50));
		itemDisplay->setString(item.name + " - " + std::to_string(item.price));
		merchandise.push_back(std::move(itemDisplay));
	}
}

void IslandMenu::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();

	// Set the position of the menu
	menu.setPosition(HUDView.getCenter().x - menu.getSize().x / 2, HUDView.getCenter().y + menu.getSize().y);
	titleText.setPosition(menu.getPosition().x + menu.getSize().x / 2 - titleText.getGlobalBounds().width / 2, menu.getPosition().y + padding);

	// Set the position of the first button to be centered on the left side of the menu rect
	interactables[0]->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 4 - interactables[0]->getSize().x / 2, menu.getPosition().y + menu.getSize().y / 2 - interactables[0]->getSize().y / 2));

	// Set the position of the second button to be centered on the right side of the menu rect
	interactables[1]->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 4 * 3 - interactables[1]->getSize().x / 2, menu.getPosition().y + menu.getSize().y / 2 - interactables[0]->getSize().y / 2));

	if (!enteredIsland) return;

	// Set the position of the market interactables
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		buyButtons[i]->setPosition(sf::Vector2f(menu.getPosition().x + padding, menu.getPosition().y + padding + interactables[0]->getSize().y + padding + (i + 1) * (padding + buyButtons[i]->getSize().y)));
		sellButtons[i]->setPosition(sf::Vector2f(menu.getPosition().x + padding + buyButtons[i]->getSize().x + padding, menu.getPosition().y + padding + interactables[0]->getSize().y + padding + (i + 1) * (padding + buyButtons[i]->getSize().y)));
		merchandise[i]->setPosition(sf::Vector2f(menu.getPosition().x + padding + buyButtons[i]->getSize().x + padding + sellButtons[i]->getSize().x + padding, menu.getPosition().y + padding + interactables[0]->getSize().y + padding + (i + 1) * (padding + buyButtons[i]->getSize().y)));
	}
	
}

void IslandMenu::draw() {
	// Draw the menu
	window->draw(menu);
	window->draw(titleText);

	// Interact
	setInteractablePositions();
	interactWithMenuItems();

	if (enteredIsland) {
		// Draw the market interactables
		for (size_t i = 0; i < buyButtons.size(); ++i) {
			buyButtons[i]->draw();
			sellButtons[i]->draw();
			merchandise[i]->draw();
		}
	}
	else {
		// Draw the inital buttons
		for (auto& interactable : interactables) {
			interactable->draw();
		}
	}
}