#include "LandMass_PG.h"

using namespace PirateGame;

void LandMass::createLandMass(LandMassType type, LandMassTextureHandler& texture) {
	// Set the type of landmass
	this->type = type;

	// Case switch for the type of landmass
	switch (type) {
		case LandMassType::Island:
			sprite.setTexture(texture.grabIslandTexture());
			sprite.setScale(1.f * scaling, 1.f * scaling);

			// Create the market for the island
			createMarket();
			islandMenu = std::make_unique<IslandMenu>(market);
			islandMenu->setUpMenu();
			break;
		case LandMassType::Rock:
			sprite.setTexture(texture.grabRockTexture());
			sprite.setScale(0.125f * scaling, 0.125f * scaling);
			break;
		case LandMassType::Shipwreck:
			break;
	}
	//setRandomRotation();
}

ShopItem& LandMass::getMarketItem(std::string itemName) {
	for (auto& it : market) {
		if (it.name == itemName) {
			return it;
		}
	}

	// If the item is not found, return the first item in the inventory
	return market.front();
}

void LandMass::createMarket() {
	std::set<std::string> itemNames = { 
		"Banana", "Cannonball", "Wood", "Gold", "Rum", "Cannon", "Sword", "Pistol", "Cloth", "Silk",
		"Spices", "Tea", "Coffee", "Tobacco", "Sugar", "Cotton", "Indigo", "Dye", "Salt", "Iron",
		"Copper", "Tin", "Lead", "Silver", "Gold", "Platinum", "Diamond", "Emerald", "Ruby", "Sapphire",
		"Pearl", "Amber", "Coral", "Ivory", "Jade", "Opal", "Quartz", "Topaz", "Turquoise", "Agate", "Garnet",
		"Lapis Lazuli", "Malachite", "Moonstone", "Obsidian", "Onyx", "Peridot", "Pyrite", "Rhodochrosite",
		"Sodalite", "Sunstone", "Tiger's Eye", "Tourmaline", "Zircon"
	};

	int marketSize = 7;

	for (int i = 0; i < marketSize; i++) {
		// Randomly select an item that is not already in the market
		std::string randItem;
		do {
			randItem = *std::next(itemNames.begin(), rand() % itemNames.size());
		} while (std::find_if(market.begin(), market.end(), [&randItem](ShopItem& item) { return item.name == randItem; }) != market.end());

		// Randomly select the price and amount
		float randPrice = (rand() % 100 + 1) * 0.1f;
		int randAmount = rand() % 100 + 1;

		// Create the item and add it to the market
		ShopItem item(randItem, randPrice, randAmount);
		market.push_back(item);
	}
}