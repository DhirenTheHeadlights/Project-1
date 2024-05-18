#include "IslandMenu_PG.h"

using namespace PirateGame;

void IslandMenu::setUpMenu() {
	// Set up the inital menu that asks the player if they wish to enter the island
	menu.setTexture(GlobalTextureHandler::getInstance().getMarketTextures().getMarketMenu());
	banner.setTexture(GlobalTextureHandler::getInstance().getMarketTextures().getIslandBanner());
	initialMenu.setTexture(GlobalTextureHandler::getInstance().getMarketTextures().getInitialMenu());

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
	"Egg Body"
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
	// Grab the global game state manager
	GSM = &GlobalGameStateManager::getInstance();

	// Initial buttons to enter the island
	std::function<void()> enterIsland = [this]() { 
		enteredIsland = true;
		ship->getMovementHandler()->setAnchorDrop(true);
	};
	std::unique_ptr<Button> enterIslandButton = std::make_unique<Button>(enterIsland);
	sf::Text enterButtonText = sf::Text("Enter Island", font, interactableTextSizeSmall);
	enterIslandButton->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getInitialMenuButton(), enterButtonText);
	enterIslandButton->getText().setFillColor(sf::Color::Black);
	addInteractableToMenu(std::move(enterIslandButton));

	std::function<void()> leaveIsland = [this]() { 
		hasPlayerSaidNo = true; // Set the flag to true so the menu does not show
		ship->getMovementHandler()->setAnchorDrop(false);
		// Enable the HUD
		GlobalValues::getInstance().setShowHUD(true);
	};
	std::unique_ptr<Button> leaveIslandButton = std::make_unique<Button>(leaveIsland);
	sf::Text leaveButtonText = sf::Text("Close", font, interactableTextSizeSmall);
	leaveIslandButton->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getInitialMenuButton(), leaveButtonText);
	leaveIslandButton->getText().setFillColor(sf::Color::Black);
	addInteractableToMenu(std::move(leaveIslandButton));

	// Add the Interactables
	addGeneralInteractables();
	addMarketInteractables();
	addShipBuyInteractables();
	addShipInventoryInteractables();
}

void IslandMenu::addGeneralInteractables() {
	// Create a text display box to show the merchant's gold
	islandGoldDisplay = std::make_unique<TextDisplayBox>();
	sf::Text goldText = sf::Text("Merchant Gold: " + std::to_string(gold), font, interactableTextSizeSmall);
	islandGoldDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getBottomRightLeft(), goldText);

	// Create a text display box to show the ship's gold
	shipGoldDisplay = std::make_unique<TextDisplayBox>();
	sf::Text shipGoldText = sf::Text("Ship Gold: ", font, interactableTextSizeSmall);
	shipGoldDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getBottomRightLeft(), shipGoldText);

	// Create an "exit" button to leave the island
	std::function<void()> closeBuyMenu = [this]() { enteredIsland = false; };
	std::unique_ptr<Button> closeBuyMenuButton = std::make_unique<Button>(closeBuyMenu);
	sf::Text closeButtonText = sf::Text("Close Menu", font, interactableTextSizeBig);
	closeBuyMenuButton->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketBottomMiddle(), closeButtonText);
	uiButtons.push_back(std::move(closeBuyMenuButton));

	// Left and right buttons to navigate each menu
	std::function<void()> leftButton = [this]() {
		if (currentPage != pages.front()) {
			currentPage = pages[static_cast<int>(currentPage) - 1];
		}
	};
	std::unique_ptr<Button> leftButtonInteractable = std::make_unique<Button>(leftButton);
	sf::Text leftButtonText = sf::Text("", font, interactableTextSizeBig);
	leftButtonInteractable->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketLeftRightNavigation(), leftButtonText);
	leftButtonInteractable->getSprite().setScale(-1.f, 1.f); // Flip the sprite
	leftRightNavButtons.push_back(std::move(leftButtonInteractable));

	std::function<void()> rightButton = [this]() {
		if (currentPage != pages.back()) {
			currentPage = pages[static_cast<int>(currentPage) + 1];
		}
	};
	std::unique_ptr<Button> rightButtonInteractable = std::make_unique<Button>(rightButton);
	sf::Text rightButtonText = sf::Text("", font, interactableTextSizeBig);
	rightButtonInteractable->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketLeftRightNavigation(), rightButtonText);
	leftRightNavButtons.push_back(std::move(rightButtonInteractable));
}

void IslandMenu::addMarketInteractables() {
	// Add every item from the market to the menu along
	// with a buy and sell button and the price of the item
	for (auto& item : market) {
		// Create the buy button
		std::function<void()> buyItem = [this, &item]() {
			if (ship->getInventoryHandler()->getGold() >= item.price && item.amount > 0) {
				// Value that determines how many are bought each second when the button is held down
				int buyRate = 1;

				// Attempt to find the item in the ship's inventory
				auto& inventory = ship->getInventoryHandler()->getInventory();
				auto it = std::find_if(inventory.begin(), inventory.end(), [&item](const ShopItem& inventoryItem) {
					return inventoryItem.name == item.name;
					});

				if (it != inventory.end()) {
					// If the item exists, update its amount
					it->amount += buyRate;
				}
				else {
					// Otherwise, add a new item with amount 1
					ShopItem newItem = item;
					newItem.amount = 1;
					inventory.push_back(newItem);
				}

				// Update gold
				ship->getInventoryHandler()->removeGold(item.price);

				// Update merchant's gold and item quantity if applicable
				item.amount -= buyRate;
				gold += item.price;
			}
		};
		std::unique_ptr<Button> buyButton = std::make_unique<Button>(buyItem);
		sf::Text buyButtonText = sf::Text("Buy", font, interactableTextSizeSmall);
		buyButton->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getBuySell(), buyButtonText);
		buyButton->setHoldDown(true);
		buyButtons.push_back(std::move(buyButton));

		// Create the sell button
		std::function<void()> sellItem = [this, &item]() {
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
		};

		std::unique_ptr<Button> sellButton = std::make_unique<Button>(sellItem);
		sf::Text sellButtonText = sf::Text("Sell", font, interactableTextSizeSmall);
		sellButton->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getBuySell(), sellButtonText);
		sellButton->setHoldDown(true);
		sellButtons.push_back(std::move(sellButton));

		// Create the text display box for the item
		std::unique_ptr<TextDisplayBox> itemDisplay = std::make_unique<TextDisplayBox>();
		sf::Text itemText = sf::Text(item.name + " - " + floatToString(item.price), font, interactableTextSizeSmall);
		itemDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketMiddle(), itemText);
		merchandise.push_back(std::move(itemDisplay));

		// Create a text display box to show the amount of the item in the market
		std::unique_ptr<TextDisplayBox> marketInventoryAmount = std::make_unique<TextDisplayBox>();
		sf::Text marketInventoryText = sf::Text(std::to_string(item.amount), font, interactableTextSizeSmall);
		marketInventoryAmount->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketFarLeftRight(), marketInventoryText);
		marketInventory.push_back(std::move(marketInventoryAmount));
	}

	// Create a box that says merchant
	marketNameDisplay = std::make_unique<TextDisplayBox>();
	sf::Text merchantText = sf::Text("Merchant", font, interactableTextSizeBig);
	marketNameDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketTopLeftRight(), merchantText);

	// Create a text display box to show the ship's name
	shipNameDisplay = std::make_unique<TextDisplayBox>();
	sf::Text shipNameText = sf::Text("Ship", font, interactableTextSizeBig);
	shipNameDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketTopLeftRight(), shipNameText);

	// Create a text display box to show the island's name
	islandNameDisplay = std::make_unique<TextDisplayBox>();
	sf::Text islandNameText = sf::Text(islandName, font, interactableTextSizeBig);
	islandNameDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketTopMiddle(), islandNameText);
}

void IslandMenu::addShipBuyInteractables() {
	// Display for the ship column
	std::unique_ptr<TextDisplayBox> shipDisplay = std::make_unique<TextDisplayBox>();
	sf::Text shipText = sf::Text("Ships", font, interactableTextSizeSmall);
	shipDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuTopLeft(), shipText);
	shipBuyTabs.push_back(std::move(shipDisplay));

	// Display for the ship's stats
	std::unique_ptr<TextDisplayBox> healthDisplay = std::make_unique<TextDisplayBox>();
	sf::Text healthText = sf::Text("Health", font, interactableTextSizeSmall);
	healthDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuTopRight(), healthText);
	shipBuyTabs.push_back(std::move(healthDisplay));

	std::unique_ptr<TextDisplayBox> speedDisplay = std::make_unique<TextDisplayBox>();
	sf::Text speedText = sf::Text("Speed", font, interactableTextSizeSmall);
	speedDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuTopRight(), speedText);
	shipBuyTabs.push_back(std::move(speedDisplay));

	std::unique_ptr<TextDisplayBox> cannonsDisplay = std::make_unique<TextDisplayBox>();
	sf::Text cannonsText = sf::Text("Cannons", font, interactableTextSizeSmall);
	cannonsDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuTopRight(), cannonsText);
	shipBuyTabs.push_back(std::move(cannonsDisplay));

	std::unique_ptr<TextDisplayBox> regenDisplay = std::make_unique<TextDisplayBox>();
	sf::Text regenText = sf::Text("Regen", font, interactableTextSizeSmall);
	regenDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuTopRight(), regenText);
	shipBuyTabs.push_back(std::move(regenDisplay));

	// Grab all of the ships from the ship enums
	for (int i = 0; i < numShipsForSale; i++) {
		// Grab a random ship class
		ShipClass shipClass = getRandomShipClass();

		std::vector<std::unique_ptr<TextDisplayBox>> shipStatsVec;

		// Create a text display box to show the ship's name
		std::unique_ptr<TextDisplayBox> shipDisplay = std::make_unique<TextDisplayBox>();
		sf::Text shipText = sf::Text(shipClassToString(shipClass), font, interactableTextSizeSmall);
		shipDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuMiddleLeft(), shipText);
		shipStatsVec.push_back(std::move(shipDisplay));

		// Create display boxes for the ship's stats. The stats displayed are the ship's health, speed, num cannons, regen rate, and cannon range
		std::unique_ptr<TextDisplayBox> healthDisplay = std::make_unique<TextDisplayBox>();
		sf::Text healthText = sf::Text(floatToString(getShipProperties(shipClass).maxHealth), font, interactableTextSizeSmall);
		healthDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuMiddleRight(), healthText);
		shipStatsVec.push_back(std::move(healthDisplay));

		std::unique_ptr<TextDisplayBox> speedDisplay = std::make_unique<TextDisplayBox>();
		sf::Text speedText = sf::Text(floatToString(getShipProperties(shipClass).baseSpeed), font, interactableTextSizeSmall);
		speedDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuMiddleRight(), speedText);
		shipStatsVec.push_back(std::move(speedDisplay));

		std::unique_ptr<TextDisplayBox> cannonsDisplay = std::make_unique<TextDisplayBox>();
		sf::Text cannonsText = sf::Text(floatToString(getShipProperties(shipClass).numCannons, 0), font, interactableTextSizeSmall);
		cannonsDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuMiddleRight(), cannonsText);
		shipStatsVec.push_back(std::move(cannonsDisplay));

		std::unique_ptr<TextDisplayBox> regenDisplay = std::make_unique<TextDisplayBox>();
		sf::Text regenText = sf::Text(floatToString(getShipProperties(shipClass).regenRate), font, interactableTextSizeSmall);
		regenDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenuMiddleRight(), regenText);
		shipStatsVec.push_back(std::move(regenDisplay));

		shipStats.push_back(std::make_pair(shipClass, std::move(shipStatsVec)));
	}
}

void IslandMenu::addShipInventoryInteractables() {
	// Add every item from the ship's inventory to the menu
	for (auto& marketItem : market) {
		auto it = std::find_if(ship->getInventoryHandler()->getInventory().begin(), ship->getInventoryHandler()->getInventory().end(), [&marketItem](const ShopItem& shipItem) {
			return shipItem.name == marketItem.name;
		});

		std::unique_ptr<TextDisplayBox> itemDisplay = std::make_unique<TextDisplayBox>();

		if (it != ship->getInventoryHandler()->getInventory().end()) {
			// Item exists in ship's inventory, display with details
			sf::Text itemText = sf::Text(std::to_string(it->amount), font, interactableTextSizeSmall);
			itemDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketFarLeftRight(), itemText);
		}
		else {
			// Item does not exist, display an empty box
			sf::Text itemText = sf::Text("Empty", font, interactableTextSizeSmall);
			itemDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketFarLeftRight(), itemText);
		}
		shipInventoryInteractable.push_back(std::move(itemDisplay));
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
		interactables[0]->setPosition(initialMenu.getPosition() + initialButtonPositionLeft);
		interactables[1]->setPosition(initialMenu.getPosition() + initialButtonPositionRight);
		
		// Title text should go in the banner and the banner should be near the top of the screen
		banner.setPosition(HUDView.getCenter().x - banner.getGlobalBounds().getSize().x / 2, HUDView.getCenter().y - static_cast<float>(window->getSize().y / 2) + padding);
		titleText.setPosition(banner.getPosition().x + banner.getGlobalBounds().width / 2 - titleText.getLocalBounds().width / 2, 
							  banner.getPosition().y + banner.getGlobalBounds().height / 2 - titleText.getLocalBounds().height / 2);

		// Set the ship stop flag to true when prompting the player to enter the island
		ship->getMovementHandler()->setAnchorDrop(true);
		ship->getMovementHandler()->setStopShipRotationFlag(true);

		// Disable the HUD
		GlobalValues::getInstance().setShowHUD(false);

		return; // Return early if the player has not entered the island
	}

	// Set up the menu to buy and sell items
	menu.setPosition(HUDView.getCenter().x - menu.getGlobalBounds().getSize().x / 2, HUDView.getCenter().y - menu.getGlobalBounds().getSize().y / 2);

	// Set the position of the title text to be in the center of the menu
	islandNameDisplay->setPosition(menu.getPosition() + marketTopMiddle);

	// Set the position of the gold info box to be right above the market interactbles
	shipGoldDisplay->setPosition(menu.getPosition() + marketBottomLeft);
	islandGoldDisplay->setPosition(menu.getPosition() + marketBottomRight);

	// Names in the top left/right
	shipNameDisplay->setPosition(menu.getPosition() + marketTopLeft);
	marketNameDisplay->setPosition(menu.getPosition() + marketTopRight);

	// Set the position of the market interactables to be in the center of the menu
	sf::Vector2f menuPos = menu.getPosition();
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		buyButtons[i]->setPosition(sf::Vector2f(menuPos.x + marketBuy.x, menuPos.y + marketBuy.y + i * buyButtons[i]->getSprite().getGlobalBounds().getSize().y));
		merchandise[i]->setPosition(sf::Vector2f(menuPos.x + marketMiddle.x, menuPos.y + marketMiddle.y + i * merchandise[i]->getSprite().getGlobalBounds().getSize().y));
		sellButtons[i]->setPosition(sf::Vector2f(menuPos.x + marketSell.x, menuPos.y + marketSell.y + i * sellButtons[i]->getSprite().getGlobalBounds().getSize().y));
		shipInventoryInteractable[i]->setPosition(sf::Vector2f(menuPos.x + marketLeftMiddle.x, menuPos.y + marketLeftMiddle.y + i * shipInventoryInteractable[i]->getSprite().getGlobalBounds().getSize().y));
		marketInventory[i]->setPosition(sf::Vector2f(menuPos.x + marketRightMiddle.x, menuPos.y + marketRightMiddle.y + i * marketInventory[i]->getSprite().getGlobalBounds().getSize().y));
	}

	// Set the positions of the left and right navigation buttons
	leftRightNavButtons[0]->setPosition(menu.getPosition() + marketNavigationLeft + sf::Vector2f(leftRightNavButtons[0]->getSprite().getGlobalBounds().width, 0.f));
	leftRightNavButtons[1]->setPosition(menu.getPosition() + marketNavigationRight);

	// Set the position of the leave island button to be at the bottom of the menu in the center
	uiButtons[0]->setPosition(menu.getPosition() + marketBottomMiddle);

	// Set the position of the ship buy interactables
	for (size_t i = 0; i < shipStats.size(); ++i) {
		shipStats[i].second[0]->setPosition(menu.getPosition() + shipBuyMiddleLeftStart + sf::Vector2f(0.f, i * shipStats[i].second[0]->getSprite().getGlobalBounds().getSize().y));
		for (size_t j = 1; j < shipStats[i].second.size(); ++j) {
			shipStats[i].second[j]->setPosition(menu.getPosition() + shipBuyMiddleRightStart + sf::Vector2f((j - 1) * shipStats[i].second[j]->getSprite().getGlobalBounds().getSize().x, i * shipStats[i].second[j]->getSprite().getGlobalBounds().getSize().y));
		}
	}

	// Set the position of the ship buy tabs
	for (size_t i = 0; i < shipBuyTabs.size(); ++i) {
		if (i == 0) shipBuyTabs[i]->setPosition(menu.getPosition() + shipBuyTopLeft); // "Ship" tab
		else shipBuyTabs[i]->setPosition(menu.getPosition() + shipBuyTopRightStart + sf::Vector2f((i - 1) * shipBuyTabs[i]->getSprite().getGlobalBounds().getSize().x, 0.f));
	}
}

void IslandMenu::updateMarket() {
	// Synchronize the local ship inventory for display purposes
	auto& currentInventory = ship->getInventoryHandler()->getInventory();

	// Update or add new boxes based on the current ship inventory
	for (size_t i = 0; i < market.size(); ++i) {
		if (i < shipInventoryInteractable.size()) {
			// Update existing box
			auto it = std::find_if(currentInventory.begin(), currentInventory.end(), [&](const ShopItem& item) {
				return item.name == market[i].name;
				});
			shipInventoryInteractable[i]->getText().setString(it != currentInventory.end() ? std::to_string(it->amount) : "Empty");
		}
		else {
			// Add new box if necessary
			std::unique_ptr<TextDisplayBox> itemDisplay = std::make_unique<TextDisplayBox>();
			sf::Text itemText = sf::Text(std::to_string(market[i].amount), font, interactableTextSizeSmall);
			itemDisplay->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketFarLeftRight(), itemText);
			auto it = std::find_if(currentInventory.begin(), currentInventory.end(), [&](const ShopItem& item) {
				return item.name == market[i].name;
				});
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
			marketInventory[i]->getText().setString(std::to_string(market[i].amount));
		}
		else {
			// Add new box if necessary
			std::unique_ptr<TextDisplayBox> marketInventoryAmount = std::make_unique<TextDisplayBox>();
			sf::Text marketInventoryText = sf::Text(market[i].name + " - " + std::to_string(market[i].amount), font, interactableTextSizeSmall);
			marketInventoryAmount->createInteractable(GlobalTextureHandler::getInstance().getMarketTextures().getMarketFarLeftRight(), marketInventoryText);
			marketInventory.push_back(std::move(marketInventoryAmount));
		}
	}

	// Update gold
	shipGoldDisplay->getText().setString("Gold: " + floatToString(ship->getInventoryHandler()->getGold()));
	islandGoldDisplay->getText().setString("Gold: " + floatToString(gold));
}

void IslandMenu::interactWithMarket() {
	// Interact with the market
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		buyButtons[i]->interact();
		sellButtons[i]->interact();
	}

	// Draw the left and right navigation buttons
	for (auto& button : leftRightNavButtons) {
		button->interact();
	}

	// Update the market
	updateMarket();
}

void IslandMenu::interactWithShipBuy() {
	// Interact with the ship buy menu
	for (size_t i = 0; i < shipStats.size(); ++i) {
		shipStats[i].second[0]->interact();
		for (size_t j = 1; j < shipStats[i].second.size(); ++j) {
			shipStats[i].second[j]->interact();
		}
	}

	// Draw the ship buy tabs
	for (auto& tab : shipBuyTabs) {
		tab->interact();
	}
}

void IslandMenu::drawMarket() {
	// Set the texture of the menu to be the market menu
	menu.setTexture(GlobalTextureHandler::getInstance().getMarketTextures().getMarketMenu());

	// Draw the market interactables
	for (size_t i = 0; i < buyButtons.size(); ++i) {
		buyButtons[i]->draw();
		sellButtons[i]->draw();
		merchandise[i]->draw();
		shipInventoryInteractable[i]->draw();
		marketInventory[i]->draw();
	}

	// Draw the other ui buttons
	for (auto& button : uiButtons) {
		button->draw();
	}

	// Draw the left and right navigation buttons
	for (auto& button : leftRightNavButtons) {
		button->draw();
	}

	// Draw information
	shipGoldDisplay->draw();
	islandGoldDisplay->draw();

	// Draw the island name
	islandNameDisplay->draw();

	// Draw the and ship name
	marketNameDisplay->draw();
	shipNameDisplay->draw();
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
			button->interact();
		}

		// Interact with the close menu button
		uiButtons[0]->interact();
	}
	else {
		// Interact with the inital menu
		for (auto& interactable : interactables) {
			interactable->interact();
		}
	}
}

void IslandMenu::drawShipBuy() {
	// Set the texture of the menu to be the ship buy menu
	menu.setTexture(GlobalTextureHandler::getInstance().getMarketTextures().getShipBuyMenu());

	// Draw the ship buy interactables
	for (size_t i = 0; i < shipStats.size(); ++i) {
		// Draw the ship name
		shipStats[i].second[0]->draw();
		shipStats[i].second[0]->getText().setFillColor(sf::Color::Black);

		// Draw the ship stats
		for (size_t j = 1; j < shipStats[i].second.size(); ++j) {
			shipStats[i].second[j]->draw();
			shipStats[i].second[j]->getText().setFillColor(sf::Color::Black);
		}
	}

	// Draw the ship buy tabs
	for (auto& tab : shipBuyTabs) {
		tab->draw();
		tab->getText().setFillColor(sf::Color::Black);
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

		uiButtons[0]->interact(); // Close menu button

		// Draw the other ui buttons
		for (auto& button : uiButtons) {
			button->draw();
		}

		// Draw the left and right navigation buttons
		for (auto& button : leftRightNavButtons) {
			button->draw();
		}

		// Draw information
		shipGoldDisplay->draw();
		islandGoldDisplay->draw();
	}
	else {
		// Draw the inital menu
		window->draw(initialMenu);

		// Draw the banner
		window->draw(banner);
		window->draw(titleText);

		// Draw the inital buttons and interact with them
		for (auto& interactable : interactables) {
			interactable->draw();
		}
	}
}