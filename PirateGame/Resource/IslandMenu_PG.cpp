#include "IslandMenu_PG.h"

using namespace PirateGame;

void IslandMenu::setUpMenu() {
	// Set up the inital menu that asks the player if they wish to enter the island
	menu.setSize(initalMenuSize);
	menu.setFillColor(sf::Color::White);
	menu.setOutlineColor(sf::Color::Black);
	menu.setOutlineThickness(2.0f);

	// Set up the island names to be names from islands in 'one piece' but a little different to avoid copyright
	std::vector<std::string> islandNames = {
	"Grand Stride",
	"Mysterious Blue",
	"Drum Summit",
	"Alabasta Kingdom",
	"Skyward Peak",
	"Water Eight",
	"Thriller Barkland",
	"Sabaody Arch",
	"Amazon Lilypond",
	"Impel Dune",
	"Marine Fordo",
	"Fish-Man Isle",
	"Punk Risk",
	"Dress Landia",
	"Whole Cake Isle",
	"Zou Haven",
	"Wano Countryard",
	"Onigashima Cove",
	"Laugh Story",
	"Elbaf Village"
	};
	
	islandName = islandNames[rand() % islandNames.size()];
	titleText = sf::Text(islandName, font, 20);
	titleText.setFillColor(sf::Color::Black);

	// Sort the market items
	std::sort(market.begin(), market.end(), [](const ShopItem& a, const ShopItem& b) {
		return a.name < b.name;
	});
	
	addInteractablesToMenu();
}

void IslandMenu::addInteractablesToMenu() {
	// Grab the global game state manager
	GSM = &GlobalValues::getInstance().getGSM();

	// Initial buttons to enter the island
	std::function<void()> enterIsland = [this]() { 
		enteredIsland = true;
		ship->getMovementHandler().setStopShipFlag(true);
	};
	std::unique_ptr<Button> enterIslandButton = std::make_unique<Button>(enterIsland);
	enterIslandButton->createInteractable(sf::Vector2f(200, 50));
	enterIslandButton->setString("Enter Island");
	addInteractableToMenu(std::move(enterIslandButton));

	std::function<void()> leaveIsland = [this]() { 
		hasPlayerSaidNo = true; // Set the flag to true so the menu does not show
		ship->getMovementHandler().setStopShipFlag(false);
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
			if (ship->getInventoryHandler().getGold() >= item.price && item.amount > 0) {
				// Attempt to find the item in the ship's inventory
				auto& inventory = ship->getInventoryHandler().getInventory();
				auto it = std::find_if(inventory.begin(), inventory.end(), [&item](const ShopItem& inventoryItem) {
					return inventoryItem.name == item.name;
					});

				if (it != inventory.end()) {
					// If the item exists, update its amount
					it->amount += 1;
				}
				else {
					// Otherwise, add a new item with amount 1
					ShopItem newItem = item;
					newItem.amount = 1;
					inventory.push_back(newItem);
				}

				// Update gold
				ship->getInventoryHandler().removeGold(item.price);

				// Update merchant's gold and item quantity if applicable
				item.amount -= 1;
				gold += item.price;
			}
		};
		std::unique_ptr<Button> buyButton = std::make_unique<Button>(buyItem);
		buyButton->createInteractable(buyButtonSize);
		buyButton->setString("Buy");
		buyButton->setHoldDown(true);
		buyButtons.push_back(std::move(buyButton));

		// Create the sell button
		std::function<void()> sellItem = [this, &item]() {
			auto& inventory = ship->getInventoryHandler().getInventory();
			auto it = std::find_if(inventory.begin(), inventory.end(), [&item](const ShopItem& inventoryItem) {
				return inventoryItem.name == item.name;
				});

			if (it != inventory.end() && it->amount > 0) {
				// If the item exists and has a positive amount, decrease the amount
				it->amount -= 1;

				// Remove the item from inventory if amount becomes 0
				if (it->amount == 0) {
					inventory.erase(it);
				}

				// Update gold
				ship->getInventoryHandler().addGold(item.price);

				// Update merchant's gold and item quantity if applicable
				item.amount += 1;
				gold -= item.price;
			}
		};

		std::unique_ptr<Button> sellButton = std::make_unique<Button>(sellItem);
		sellButton->createInteractable(sellButtonSize);
		sellButton->setString("Sell");
		sellButton->setHoldDown(true);
		sellButtons.push_back(std::move(sellButton));

		// Create the text display box for the item
		std::unique_ptr<TextDisplayBox> itemDisplay = std::make_unique<TextDisplayBox>();
		itemDisplay->createInteractable(merchandiseSize);
		itemDisplay->setString(item.name + " - " + floatToString(item.price));
		merchandise.push_back(std::move(itemDisplay));

		// Create a text display box to show the amount of the item in the market
		std::unique_ptr<TextDisplayBox> marketInventoryAmount = std::make_unique<TextDisplayBox>();
		marketInventoryAmount->createInteractable(merchandiseSize);
		marketInventoryAmount->setString(item.name + " - " + std::to_string(item.amount));
		marketInventory.push_back(std::move(marketInventoryAmount));
	}

	// Create a text display box to show the merchant's gold
	islandGoldDisplay = std::make_unique<TextDisplayBox>();
	islandGoldDisplay->createInteractable(goldDisplaySize);
	islandGoldDisplay->setString("Merchant Gold: " + std::to_string(gold));

	// Create a text display box to show the ship's gold
	shipGoldDisplay = std::make_unique<TextDisplayBox>();
	shipGoldDisplay->createInteractable(goldDisplaySize);
	shipGoldDisplay->setString("Ship Gold: "); // Will be updated in the draw function

	// Create a text display box to show the island's name
	islandNameDisplay = std::make_unique<TextDisplayBox>();
	islandNameDisplay->createInteractable(islandNameDisplaySize);
	islandNameDisplay->setString(islandName);

	// Create an "exit" button to leave the island
	std::function<void()> leaveIsland = [this]() { enteredIsland = false; ship->getMovementHandler().setStopShipFlag(false); };
	std::unique_ptr<Button> leaveIslandButton = std::make_unique<Button>(leaveIsland);
	leaveIslandButton->createInteractable(uiButtonSize);
	leaveIslandButton->setString("Leave Island");
	uiButtons.push_back(std::move(leaveIslandButton));
}

void IslandMenu::addShipInventoryInteractables() {
	// Add every item from the ship's inventory to the menu
	for (auto& marketItem : market) {
		auto it = std::find_if(ship->getInventoryHandler().getInventory().begin(), ship->getInventoryHandler().getInventory().end(), [&marketItem](const ShopItem& shipItem) {
			return shipItem.name == marketItem.name;
		});

		std::unique_ptr<TextDisplayBox> itemDisplay = std::make_unique<TextDisplayBox>();
		itemDisplay->createInteractable(merchandiseSize);

		if (it != ship->getInventoryHandler().getInventory().end()) {
			// Item exists in ship's inventory, display with details
			itemDisplay->setString(it->name + " - " + std::to_string(it->amount));
		}
		else {
			// Item does not exist, display an empty box
			itemDisplay->setString("Empty");
		}
		shipInventoryInteractable.push_back(std::move(itemDisplay));
	}
	this->shipInventory = ship->getInventoryHandler().getInventory();
	addedShipInventory = true;
}

void IslandMenu::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();

	// Basically, we want the menu to not show if the player pressed the "leave island" button
	// This flag is set to false for all landmasses not near the player, and is set true when the player
	// selects the "enter island" button. The flag will be reset in the landmass handler.
	if (hasPlayerSaidNo) return;

	if (!enteredIsland) {
		// Set the position of the menu (only the inital menu)
		menu.setSize(initalMenuSize); // Reset the size of the menu
		menu.setPosition(HUDView.getCenter().x - menu.getSize().x / 2, HUDView.getCenter().y + menu.getSize().y);
		titleText.setPosition(menu.getPosition().x + menu.getSize().x / 2 - titleText.getGlobalBounds().width / 2, menu.getPosition().y + padding);

		// Set the position of the first button to be centered on the left side of the menu rect
		interactables[0]->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 4 - interactables[0]->getSize().x / 2, menu.getPosition().y + menu.getSize().y / 2 - interactables[0]->getSize().y / 4));

		// Set the position of the second button to be centered on the right side of the menu rect
		interactables[1]->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 4 * 3 - interactables[1]->getSize().x / 2, menu.getPosition().y + menu.getSize().y / 2 - interactables[0]->getSize().y / 4));

		// Set the ship stop flag to true when prompting the player to enter the island
		ship->getMovementHandler().setStopShipFlag(true);
		ship->getMovementHandler().setStopShipRotationFlag(true);

		return; // Return early if the player has not entered the island
	}

	// Set up the menu to buy and sell items
	menu.setSize(expandedMenuSize);
	menu.setPosition(HUDView.getCenter().x - menu.getSize().x / 2, HUDView.getCenter().y - menu.getSize().y / 2);

	// Set the position of the title text to be in the center of the menu
	islandNameDisplay->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 2.f - islandNameDisplay->getSize().x / 2.f, menu.getPosition().y + 0.25f * menu.getSize().y - goldDisplaySize.y - 3.5 * padding));

	// Set the position of the gold info box to be right above the market interactbles
	shipGoldDisplay->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 2.f - merchandiseSize.x / 2 - buyButtonSize.x - merchandiseSize.x - 2 * padding, menu.getPosition().y + 0.25f * menu.getSize().y - goldDisplaySize.y - 3.5 * padding));
	islandGoldDisplay->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 2.f + merchandiseSize.x / 2 + buyButtonSize.x + 2 * padding, menu.getPosition().y + 0.25f * menu.getSize().y - goldDisplaySize.y - 3.5 * padding));

	// Set the position of the market interactables to be in the center of the menu
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		float buttonX = menu.getPosition().x + menu.getSize().x / 2.f - merchandiseSize.x / 2.f - buyButtonSize.x - padding;
		float buttonY = menu.getPosition().y + 0.25f * menu.getSize().y - buyButtons[i]->getSize().y / 2.f + i * (buyButtons[i]->getSize().y + padding);
		buyButtons[i]->setPosition(sf::Vector2f(buttonX, buttonY));

		buttonX = menu.getPosition().x + menu.getSize().x / 2 - merchandise[i]->getSize().x / 2.f;
		buttonY = menu.getPosition().y + 0.25 * menu.getSize().y - merchandise[i]->getSize().y / 2.f + i * (merchandise[i]->getSize().y + padding);
		merchandise[i]->setPosition(sf::Vector2f(buttonX, buttonY));

		buttonX = menu.getPosition().x + menu.getSize().x / 2.f + merchandiseSize.x / 2.f + padding;
		buttonY = menu.getPosition().y + 0.25f * menu.getSize().y - sellButtons[i]->getSize().y / 2.f + i * (sellButtons[i]->getSize().y + padding);
		sellButtons[i]->setPosition(sf::Vector2f(buttonX, buttonY));

		buttonX = menu.getPosition().x + menu.getSize().x / 2.f - merchandiseSize.x / 2.f - buyButtonSize.x - merchandiseSize.x - 2 * padding;
		buttonY = menu.getPosition().y + 0.25f * menu.getSize().y - shipInventoryInteractable[i]->getSize().y / 2.f + i * (shipInventoryInteractable[i]->getSize().y + padding);
		shipInventoryInteractable[i]->setPosition(sf::Vector2f(buttonX, buttonY));

		buttonX = menu.getPosition().x + menu.getSize().x / 2.f + merchandiseSize.x / 2.f + buyButtonSize.x + 2.f * padding;
		buttonY = menu.getPosition().y + 0.25f * menu.getSize().y - marketInventory[i]->getSize().y / 2.f + i * (marketInventory[i]->getSize().y + padding);
		marketInventory[i]->setPosition(sf::Vector2f(buttonX, buttonY));
	}

	// Set the position of the leave island button to be at the bottom of the menu in the center
	uiButtons[0]->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x / 2.f - uiButtons[0]->getSize().x / 2.f, menu.getPosition().y + menu.getSize().y - uiButtons[0]->getSize().y - padding));
}

void IslandMenu::updateMarket() {
	// Synchronize the local ship inventory for display purposes
	auto& currentInventory = ship->getInventoryHandler().getInventory();

	// Update or add new boxes based on the current ship inventory
	for (size_t i = 0; i < market.size(); ++i) {
		if (i < shipInventoryInteractable.size()) {
			// Update existing box
			auto it = std::find_if(currentInventory.begin(), currentInventory.end(), [&](const ShopItem& item) {
				return item.name == market[i].name;
				});
			shipInventoryInteractable[i]->setString(it != currentInventory.end() ? it->name + " - " + std::to_string(it->amount) : "Empty");
		}
		else {
			// Add new box if necessary
			std::unique_ptr<TextDisplayBox> itemDisplay = std::make_unique<TextDisplayBox>();
			itemDisplay->createInteractable(merchandiseSize);
			auto it = std::find_if(currentInventory.begin(), currentInventory.end(), [&](const ShopItem& item) {
				return item.name == market[i].name;
				});
			itemDisplay->setString(it != currentInventory.end() ? it->name + " - " + std::to_string(it->amount) : "Empty");
			shipInventoryInteractable.push_back(std::move(itemDisplay));
		}
	}

	// Remove excess boxes if the current inventory is smaller than the number of boxes
	if (market.size() < shipInventoryInteractable.size()) {
		shipInventoryInteractable.resize(market.size());
	}

	// Update or add new boxes based on the current item amount in  the market
	for (size_t i = 0; i < market.size(); ++i) {
		if (i < marketInventory.size()) {
			// Update existing box
			marketInventory[i]->setString(market[i].name + " - " + std::to_string(market[i].amount));
		}
		else {
			// Add new box if necessary
			std::unique_ptr<TextDisplayBox> marketInventoryAmount = std::make_unique<TextDisplayBox>();
			marketInventoryAmount->createInteractable(merchandiseSize);
			marketInventoryAmount->setString(market[i].name + " - " + std::to_string(market[i].amount));
			marketInventory.push_back(std::move(marketInventoryAmount));
		}
	}
}


void IslandMenu::draw() {
	// if the player has said no, do not draw the menu
	if (hasPlayerSaidNo) return;

	// Draw the menu
	window->draw(menu);

	// Draw the ship inventory if the player has entered the island
	if (enteredIsland && !addedShipInventory) {
		addShipInventoryInteractables();
	}

	setInteractablePositions();

	if (enteredIsland) {
		// Draw the market interactables
		for (size_t i = 0; i < buyButtons.size(); ++i) {
			buyButtons[i]->draw();
			sellButtons[i]->draw();
			merchandise[i]->draw();
			shipInventoryInteractable[i]->draw();
			marketInventory[i]->draw();
		}

		// Interact with the market
		for (size_t i = 0; i < buyButtons.size(); ++i) {
			buyButtons[i]->interact();
			sellButtons[i]->interact();
			uiButtons[0]->interact();
		}

		// Draw the other ui buttons
		for (auto& button : uiButtons) {
			button->getFrame().setFillColor(sf::Color::Transparent);
			button->getFrame().setOutlineThickness(0);
			button->setPadding(2.f);
			button->getForeground().setSize(sf::Vector2f(uiButtonSize.x - 2 * button->getPadding(), uiButtonSize.y - 2 * button->getPadding()));
			button->getText().setCharacterSize(static_cast<unsigned int>(15));
			button->draw();
		}

		// Update market
		updateMarket();

		// Update and draw the gold text
		shipGoldDisplay->getText().setString("Ship Gold: " + floatToString(ship->getInventoryHandler().getGold()));
		shipGoldDisplay->draw();

		islandGoldDisplay->getText().setString("Merchant Gold: " + floatToString(gold));
		islandGoldDisplay->draw();

		// Draw the island name
		islandNameDisplay->draw();
	}
	else {
		// Draw the inital buttons and interact with them
		for (auto& interactable : interactables) {
			interactable->draw();
			interactable->interact();
		}
		window->draw(titleText);
	}
}