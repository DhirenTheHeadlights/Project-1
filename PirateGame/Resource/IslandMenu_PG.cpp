#include "IslandMenu_PG.h"

using namespace PirateGame;

void IslandMenu::setUpMenu() {
	// Set up the inital menu that asks the player if they wish to enter the island
	menu.setTexture(Textures::marketTextures.getMarketMenu());
	banner.setTexture(Textures::marketTextures.getIslandBanner());
	initialMenu.setTexture(Textures::marketTextures.getInitialMenu());

	// Set up the island names to be names from islands in 'one piece' but a little different to avoid copyright
	std::vector<std::string> islandNames = {
	"Grand Stride",
	"Mysterious Blue",
	"Drum Summit",
	"Alabasta Kingdom",
	"Skyward Peak",
	"Water Eight",
	"Thriller Barkland",
	"Saboady Arch",
	"Amazon Lilypond",
	"Impel Dune",
	"Marine Forth",
	"Fish-Man Aisle",
	"Classical Risk",
	"Naked-rossa",
	"Half Cake Isle",
	"uoZ",
	"Wano Countryard",
	"Onigashima Cove",
	"Elbalf Village",
	"Raftel",
	"Egg Body",
	"Arrak Island"
	};
	
	islandName = islandNames[rand() % islandNames.size()];
	titleText = sf::Text(islandName, font, interactableTextSizeBig);
	titleText.setOrigin(0.f, 0.f);
	titleText.setFillColor(sf::Color::Black);

	// Sort the market items
	std::sort(market.begin(), market.end(), [](const ShopItem& a, const ShopItem& b) {
		return a.name < b.name;
	});
}

void IslandMenu::addInteractablesToMenu() {
	// Initial buttons to enter the island
	addButton(sf::Text("Island Menu", font, interactableTextSizeSmall), Textures::marketTextures.getInitialMenuButton(), initialButtons, [this]() {
		// Set the flag to true so the menu shows
		enteredIsland = true;
		ship->getMovementHandler()->setAnchorDrop(true);
	});
	initialButtons[0].getText().setFillColor(sf::Color::Black);

	addButton(sf::Text("Close", font, interactableTextSizeSmall), Textures::marketTextures.getInitialMenuButton(), initialButtons, [this]() {
		hasPlayerSaidNo = true; // Set the flag to true so the menu does not show
		ship->getMovementHandler()->setAnchorDrop(false);
		// Enable the HUD
		Globals::showHUD = true;
	});
	initialButtons[1].getText().setFillColor(sf::Color::Black);

	// Add the Interactables
	addGeneralInteractables();
	addMarketInteractables();
	addShipBuyInteractables();
	addShipInventoryInteractables();
}

void IslandMenu::addGeneralInteractables() {
	// Create a text display box to show the merchant's gold
	sf::Text goldText = sf::Text("Merchant Gold: " + std::to_string(gold), font, interactableTextSizeSmall);
	islandGoldDisplay.createInteractable(Textures::marketTextures.getBottomRightLeft(), goldText);

	// Create a text display box to show the ship's gold
	sf::Text shipGoldText = sf::Text("Ship Gold: ", font, interactableTextSizeSmall);
	shipGoldDisplay.createInteractable(Textures::marketTextures.getBottomRightLeft(), shipGoldText);

	// Create an "exit" button to leave the island
	addButton(sf::Text("Close Menu", font, interactableTextSizeSmall), Textures::marketTextures.getMarketBottomMiddle(), uiButtons, [this]() {
		enteredIsland = false;
	});

	// Left and right buttons to navigate each menu 
	addButton(sf::Text("", font, interactableTextSizeBig), Textures::marketTextures.getMarketLeftRightNavigation(), leftRightNavButtons, [this]() {
		if (currentPage != pages.front()) {
			currentPage = pages[static_cast<std::vector<PirateGame::Page, std::allocator<PirateGame::Page>>::size_type>(static_cast<int>(currentPage)) - 1];
		}
	});
	leftRightNavButtons[0].getSprite().setScale(-1.f, 1.f); // Flip the sprite to face left
	addButton(sf::Text("", font, interactableTextSizeBig), Textures::marketTextures.getMarketLeftRightNavigation(), leftRightNavButtons, [this]() {
		if (currentPage != pages.back()) {
			currentPage = pages[static_cast<std::vector<PirateGame::Page, std::allocator<PirateGame::Page>>::size_type>(static_cast<int>(currentPage)) + 1];
		}
	});
}

void IslandMenu::addMarketInteractables() {
	// Add every item from the market to the menu along
	// with a buy and sell button and the price of the item
	for (auto& item : market) {
		// Create the buy button
		addButton(sf::Text("Buy", font, interactableTextSizeSmall), Textures::marketTextures.getBuySell(), buyButtons, [this, &item]() {
			if (ship->getInventoryHandler()->getGold() >= item.price && item.amount > 0) {
				// Attempt to find the item in the ship's inventory
				auto& inventory = ship->getInventoryHandler()->getInventory();
				auto it = std::find_if(inventory.begin(), inventory.end(), [&item](const ShopItem& inventoryItem) {
					return inventoryItem.name == item.name;
					});

				if (it != inventory.end()) {
					// If the item exists, update its amount
					it->amount += 1;
					std::cout << item.name << " successfully incremented by 1." << std::endl;
				}
				else {
					// Otherwise, add a new item with amount 1
					ShopItem newItem = item;
					newItem.amount = 1;
					inventory.push_back(newItem);
					std::cout << item.name << " successfully added to inventory." << std::endl;
				}
				for (auto& item : inventory) { std::cout << item.name << ": " << std::to_string(item.amount) << std::endl; }

				// Update gold
				ship->getInventoryHandler()->removeGold(item.price);

				// Update merchant's gold and item quantity if applicable
				item.amount -= 1;
				gold += item.price;
			}
		});

		// Create the sell button
		addButton(sf::Text("Sell", font, interactableTextSizeSmall), Textures::marketTextures.getBuySell(), sellButtons, [this, &item]() {
			auto& inventory = ship->getInventoryHandler()->getInventory();
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
				ship->getInventoryHandler()->addGold(item.price);

				// Update merchant's gold and item quantity if applicable
				item.amount += 1;
				gold -= item.price;
			}
		});

		// Create the text display box for the item
		addTextDisplayBox(sf::Text(item.name + " - " + floatToString(item.price), font, interactableTextSizeSmall), Textures::marketTextures.getMarketMiddle(), merchandise);

		// Create a text display box to show the amount of the item in the market
		addTextDisplayBox(sf::Text(std::to_string(item.amount), font, interactableTextSizeSmall), Textures::marketTextures.getMarketFarLeftRight(), marketInventory);
	}

	// Set hold down to be true for the buy and sell buttons
	for (auto& button : buyButtons) {
		button.setHoldDown(true);
	}
	for (auto& button : sellButtons) {
		button.setHoldDown(true);
	}

	// Create a box that says merchant
	sf::Text merchantText = sf::Text("Merchant", font, interactableTextSizeBig);
	marketNameDisplay.createInteractable(Textures::marketTextures.getMarketTopLeftRight(), merchantText);

	// Create a text display box to show the ship's name
	sf::Text shipNameText = sf::Text("Ship", font, interactableTextSizeBig);
	shipNameDisplay.createInteractable(Textures::marketTextures.getMarketTopLeftRight(), shipNameText);

	// Create a text display box to show the island's name
	sf::Text islandNameText = sf::Text(islandName, font, interactableTextSizeBig);
	islandNameDisplay.createInteractable(Textures::marketTextures.getMarketTopMiddle(), islandNameText);
}

void IslandMenu::addShipBuyInteractables() {
	// Display for the ship column
	addTextDisplayBox(sf::Text("Ships", font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuTopLeft(), shipBuyTabs);

	// Displays for the ship's stats
	addTextDisplayBox(sf::Text("Health", font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuTopRight(), shipBuyTabs);
	addTextDisplayBox(sf::Text("Speed", font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuTopRight(), shipBuyTabs);
	addTextDisplayBox(sf::Text("Cannons", font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuTopRight(), shipBuyTabs);
	addTextDisplayBox(sf::Text("Regen", font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuTopRight(), shipBuyTabs);
	addTextDisplayBox(sf::Text("Price", font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuTopRight(), shipBuyTabs);

	// Grab all of the ships from the ship enums
	for (int i = 0; i < numShipsForSale; i++) {
		// Grab a random ship class
		ShipClass shipClass = getRandomShipClass();

		std::vector<TextDisplayBox> shipStatsVec;

		// Create a text display box to show the ship's name
		addTextDisplayBox(sf::Text(shipClassToString(shipClass), font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuMiddleLeft(), shipStatsVec);

		// Create display boxes for the ship's stats. The stats displayed are the ship's health, speed, num cannons, regen rate, and price
		addTextDisplayBox(sf::Text(floatToString(getShipProperties(shipClass).maxHealth), font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuMiddleRight(), shipStatsVec);
		addTextDisplayBox(sf::Text(floatToString(getShipProperties(shipClass).baseSpeed), font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuMiddleRight(), shipStatsVec);
		addTextDisplayBox(sf::Text(std::to_string(getShipProperties(shipClass).numCannons), font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuMiddleRight(), shipStatsVec);
		addTextDisplayBox(sf::Text(floatToString(getShipProperties(shipClass).regenRate), font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuMiddleRight(), shipStatsVec);
		addTextDisplayBox(sf::Text(floatToString(getShipProperties(shipClass).price), font, interactableTextSizeSmall), Textures::marketTextures.getShipBuyMenuMiddleRight(), shipStatsVec);

		// Add a buy button for the ship
		addButton(sf::Text("Buy", font, interactableTextSizeSmall), Textures::marketTextures.getBuySell(), shipBuyButtons, [this, shipClass]() {
			if (ship->getInventoryHandler()->getGold() >= getShipProperties(shipClass).price) {
				// Set the ship's class to the new ship class
				ship->changeShipClass(shipClass);

				// Update gold
				ship->getInventoryHandler()->removeGold(getShipProperties(shipClass).price);
			}
		});

		shipStats.push_back(std::make_pair(shipClass, std::move(shipStatsVec)));
	}
}

void IslandMenu::addShipInventoryInteractables() {
	// Add every item from the ship's inventory to the menu
	for (auto& marketItem : market) {
		auto it = std::find_if(ship->getInventoryHandler()->getInventory().begin(), ship->getInventoryHandler()->getInventory().end(), [&marketItem](const ShopItem& shipItem) {
			return shipItem.name == marketItem.name;
		});

		if (it != ship->getInventoryHandler()->getInventory().end()) {
			// Item exists in ship's inventory, display with details
			addTextDisplayBox(sf::Text(it->name + " - " + std::to_string(it->amount), font, interactableTextSizeSmall), Textures::marketTextures.getMarketFarLeftRight(), shipInventoryInteractables);
		}
		else {
			// Item does not exist, display an empty box
			addTextDisplayBox(sf::Text("Empty", font, interactableTextSizeSmall), Textures::marketTextures.getMarketFarLeftRight(), shipInventoryInteractables);
		}
	}
	this->shipInventory = ship->getInventoryHandler()->getInventory();
}

void IslandMenu::setInteractablePositions() {
	// Get the view position
	sf::View HUDView = window->getView();

	// Basically, we want the menu to not show if the player pressed the "leave island" button
	// This flag is set to false for all landmasses not near the player, and is set true when the player
	// selects the "enter island" button. The flag will be reset in the landmass handler when the player
	// moves away from the landmass
	if (hasPlayerSaidNo) return;

	if (!enteredIsland) {
		// Set the position of the menu (only the inital menu) to be near the bottom of the screen to not get in the way of the player
		initialMenu.setPosition(HUDView.getCenter().x - initialMenu.getGlobalBounds().getSize().x / 2, HUDView.getCenter().y + static_cast<float>(window->getSize().y / 4));
		initialButtons[0].setPosition(initialMenu.getPosition() + initialButtonPositionLeft);
		initialButtons[1].setPosition(initialMenu.getPosition() + initialButtonPositionRight);
		
		// Title text should go in the banner and the banner should be near the top of the screen
		banner.setPosition(HUDView.getCenter().x - banner.getGlobalBounds().getSize().x / 2, HUDView.getCenter().y - static_cast<float>(window->getSize().y / 2) + padding);
		titleText.setPosition(banner.getPosition().x + banner.getGlobalBounds().width / 2 - titleText.getLocalBounds().width / 2, 
							  banner.getPosition().y + banner.getGlobalBounds().height / 2 - titleText.getLocalBounds().height / 2);

		// Set the ship stop flag to true when prompting the player to enter the island
		ship->getMovementHandler()->setAnchorDrop(true);
		ship->getMovementHandler()->setStopShipRotationFlag(true);

		// Disable the HUD
		Globals::showHUD = false;

		return; // Return early if the player has not entered the island
	}

	// Set up the menu to buy and sell items
	menu.setPosition(HUDView.getCenter().x - menu.getGlobalBounds().getSize().x / 2, HUDView.getCenter().y - menu.getGlobalBounds().getSize().y / 2);

	// Set the position of the title text to be in the center of the menu
	islandNameDisplay.setPosition(menu.getPosition() + marketTopMiddle);

	// Set the position of the gold info box to be right above the market interactbles
	shipGoldDisplay.setPosition(menu.getPosition() + marketBottomLeft);
	islandGoldDisplay.setPosition(menu.getPosition() + marketBottomRight);

	// Names in the top left/right
	shipNameDisplay.setPosition(menu.getPosition() + marketTopLeft);
	marketNameDisplay.setPosition(menu.getPosition() + marketTopRight);

	// Set the position of the market interactables to be in the center of the menu
	sf::Vector2f menuPos = menu.getPosition();
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		buyButtons[i].setPosition(sf::Vector2f(menuPos.x + marketBuy.x, menuPos.y + marketBuy.y + i * buyButtons[i].getSprite().getGlobalBounds().getSize().y));
		merchandise[i].setPosition(sf::Vector2f(menuPos.x + marketMiddle.x, menuPos.y + marketMiddle.y + i * merchandise[i].getSprite().getGlobalBounds().getSize().y));
		sellButtons[i].setPosition(sf::Vector2f(menuPos.x + marketSell.x, menuPos.y + marketSell.y + i * sellButtons[i].getSprite().getGlobalBounds().getSize().y));
		shipInventoryInteractables[i].setPosition(sf::Vector2f(menuPos.x + marketLeftMiddle.x, menuPos.y + marketLeftMiddle.y + i * shipInventoryInteractables[i].getSprite().getGlobalBounds().getSize().y));
		marketInventory[i].setPosition(sf::Vector2f(menuPos.x + marketRightMiddle.x, menuPos.y + marketRightMiddle.y + i * marketInventory[i].getSprite().getGlobalBounds().getSize().y));
	}

	// Set the positions of the left and right navigation buttons
	leftRightNavButtons[0].setPosition(menu.getPosition() + marketNavigationLeft + sf::Vector2f(leftRightNavButtons[0].getSprite().getGlobalBounds().width, 0.f));
	leftRightNavButtons[1].setPosition(menu.getPosition() + marketNavigationRight);

	// Set the position of the leave island button to be at the bottom of the menu in the center
	uiButtons[0].setPosition(menu.getPosition() + marketBottomMiddle);

	// Set the position of the ship buy interactables
	for (size_t i = 0; i < shipStats.size(); ++i) {
		shipStats[i].second[0].setPosition(menu.getPosition() + shipBuyMiddleLeftStart + sf::Vector2f(0.f, i * shipStats[i].second[0].getSprite().getGlobalBounds().getSize().y));
		for (size_t j = 1; j < shipStats[i].second.size(); ++j) {
			shipStats[i].second[j].setPosition(menu.getPosition() + shipBuyMiddleRightStart + sf::Vector2f((j - 1) * shipStats[i].second[j].getSprite().getGlobalBounds().getSize().x, i * shipStats[i].second[j].getSprite().getGlobalBounds().getSize().y));
		}
	}

	// Set the position of the buy buttons for the ships
	for (size_t i = 0; i < shipBuyButtons.size(); ++i) {
		shipBuyButtons[i].setPosition(menu.getPosition() + shipBuyMiddleRightStart + sf::Vector2f(shipStats[0].second[0].getSprite().getGlobalBounds().getSize().x * static_cast<float>(shipStats.size()), i * shipBuyButtons[i].getSprite().getGlobalBounds().getSize().y));
	}

	// Set the position of the ship buy tabs
	for (size_t i = 0; i < shipBuyTabs.size(); ++i) {
		if (i == 0) shipBuyTabs[i].setPosition(menu.getPosition() + shipBuyTopLeft); // "Ship" tab
		else shipBuyTabs[i].setPosition(menu.getPosition() + shipBuyTopRightStart + sf::Vector2f((i - 1) * shipBuyTabs[i].getSprite().getGlobalBounds().getSize().x, 0.f));
	}
}

void IslandMenu::updateMarket() {
	// Synchronize the local ship inventory for display purposes
	auto& currentInventory = ship->getInventoryHandler()->getInventory();

	// Update or add new boxes based on the current ship inventory
	for (size_t i = 0; i < market.size(); ++i) {
		if (i < shipInventoryInteractables.size()) {
			// Update existing box
			auto it = std::find_if(currentInventory.begin(), currentInventory.end(), [&](const ShopItem& item) {
				return item.name == market[i].name;
				});
			shipInventoryInteractables[i].getText().setString(it != currentInventory.end() ? std::to_string(it->amount) : "Empty");
		}
		else {
			// Add new box if necessary
			addTextDisplayBox(sf::Text(std::to_string(market[i].amount), font, interactableTextSizeSmall), Textures::marketTextures.getMarketFarLeftRight(), shipInventoryInteractables);
		}
	}

	// Remove excess boxes if the current inventory is smaller than the number of boxes
	if (market.size() < shipInventoryInteractables.size()) {
		shipInventoryInteractables.resize(market.size());
	}

	// Update or add new boxes based on the current item amount in  the market
	for (size_t i = 0; i < market.size(); ++i) {
		if (i < marketInventory.size()) {
			// Update existing box
			marketInventory[i].getText().setString(std::to_string(market[i].amount));
		}
		else {
			// Add new box if necessary
			addTextDisplayBox(sf::Text(market[i].name + " - " + std::to_string(market[i].amount), font, interactableTextSizeSmall), Textures::marketTextures.getMarketFarLeftRight(), marketInventory);
		}
	}

	// Update gold
	shipGoldDisplay.getText().setString("Gold: " + floatToString(ship->getInventoryHandler()->getGold()));
	islandGoldDisplay.getText().setString("Gold: " + floatToString(gold));
}

void IslandMenu::interactWithMarket() {
	// Interact with the market
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		buyButtons[i].interact(window,  context.GSM.get());
		sellButtons[i].interact(window,  context.GSM.get());
	}

	// Interact with the left and right navigation buttons
	for (auto& button : leftRightNavButtons) {
		button.interact(window,  context.GSM.get());
	}

	// Update the market
	updateMarket();
}

void IslandMenu::interactWithShipBuy() {
	// Interact with the ship buy menu
	for (size_t i = 0; i < shipBuyButtons.size(); ++i) {
		shipBuyButtons[i].interact(window,  context.GSM.get());
	}
}

void IslandMenu::drawMarket() {
	// Set the texture of the menu to be the market menu
	menu.setTexture(Textures::marketTextures.getMarketMenu());

	// Draw the market interactables
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		buyButtons[i].draw(window);
		sellButtons[i].draw(window);
		merchandise[i].draw(window);
		shipInventoryInteractables[i].draw(window);
		marketInventory[i].draw(window);
	}

	// Draw the other ui buttons
	for (auto& button : uiButtons) {
		button.draw(window);
	}

	// Draw the left and right navigation buttons
	for (auto& button : leftRightNavButtons) {
		button.draw(window);
	}

	// Draw information
	shipGoldDisplay.draw(window);
	islandGoldDisplay.draw(window);

	// Draw the island name
	islandNameDisplay.draw(window);

	// Draw the and ship name
	marketNameDisplay.draw(window);
	shipNameDisplay.draw(window);
}

void IslandMenu::interactWithMenuItems() {
	// if the player has said no, do not interact with the menu
	if (hasPlayerSaidNo) return;

	if (enteredIsland) {
		switch (currentPage) {
		case Page::marketPage1:
			interactWithMarket();
			break;
		case Page::shipUpgrades:
			interactWithShipBuy();
			break;
		}

		// Interact with the left and right navigation buttons
		for (auto& button : leftRightNavButtons) {
			button.interact(window,  context.GSM.get());
		}

		// Interact with the close menu button
		uiButtons[0].interact(window,  context.GSM.get());
	}
	else {
		// Interact with the inital menu
		for (auto& interactable : initialButtons) {
			interactable.interact(window,  context.GSM.get());
		}
	}
}

void IslandMenu::drawShipBuy() {
	// Set the texture of the menu to be the ship buy menu
	menu.setTexture(Textures::marketTextures.getShipBuyMenu());

	// Draw the ship buy interactables
	for (size_t i = 0; i < shipStats.size(); ++i) {
		// Draw the ship name
		shipStats[i].second[0].draw(window);
		shipStats[i].second[0].getText().setFillColor(sf::Color::Black);

		// Draw the ship stats
		for (size_t j = 1; j < shipStats[i].second.size(); ++j) {
			shipStats[i].second[j].draw(window);
			shipStats[i].second[j].getText().setFillColor(sf::Color::Black);
		}
	}

	// Draw the buy buttons for the ships
	for (auto& button : shipBuyButtons) {
		button.draw(window);
		button.getText().setFillColor(sf::Color::Black);
	}

	// Draw the ship buy tabs
	for (auto& tab : shipBuyTabs) {
		tab.draw(window);
		tab.getText().setFillColor(sf::Color::Black);
	}
}

void IslandMenu::update() {
	if (hasPlayerSaidNo) return;

	// Add Interactables if they have not been added
	if (!addedInteractables) {
		addInteractablesToMenu();
		addedInteractables = true;
	}

	setInteractablePositions();

	interactWithMenuItems();
}

void IslandMenu::draw() {
	// if the player has said no, do not draw the menu
	if (hasPlayerSaidNo) return;

	if (enteredIsland) {
		// Draw menu
		window->draw(menu);

		// Draw based on the current page
		switch (currentPage) {
		case Page::marketPage1:
			drawMarket();
			break;
		case Page::shipUpgrades:
			drawShipBuy();
			break;
		}

		// Draw the other ui buttons
		for (auto& button : uiButtons) {
			button.draw(window);
		}

		// Draw the left and right navigation buttons
		for (auto& button : leftRightNavButtons) {
			button.draw(window);
		}

		// Draw information
		shipGoldDisplay.draw(window);
		islandGoldDisplay.draw(window);
	}
	else {
		// Draw the inital menu
		window->draw(initialMenu);

		// Draw the banner
		window->draw(banner);
		window->draw(titleText);

		// Draw the inital buttons and interact with them
		for (auto& interactable : initialButtons) {
			interactable.draw(window);
		}
	}
}