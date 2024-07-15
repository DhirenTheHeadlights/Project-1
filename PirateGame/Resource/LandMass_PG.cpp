#include "LandMass_PG.h"

using namespace PirateGame;

void LandMass::createLandMass(LandMassType type) {
	this->type = type;
	std::cout << "type: " << static_cast<int>(type) << std::endl;
	switch (type) {
	case LandMassType::Island: {
		std::cout << "treating as island" << std::endl;
		LandMassTypeGenerator<IslandType> typeGen;
		specificType = typeGen.getType();
		sprite.setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getIslandTextures().getTexture(std::get<IslandType>(specificType)));
		sprite.setScale(1.f * scaling, 1.f * scaling);

		market = createLootPool(marketSizeLimit, marketPriceLimit, marketItemLimit);
		islandMenu = std::make_unique<IslandMenu>(market);
		islandMenu->setUpMenu();

		break;
	}
	case LandMassType::Rock: {
		LandMassTypeGenerator<RockType> typeGen;
		specificType = typeGen.getType();
		sprite.setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getRockTextures().getTexture(std::get<RockType>(specificType)));
		sprite.setScale(0.125f * scaling, 0.125f * scaling);
		break;
	}
	case LandMassType::Shipwreck:
		LandMassTypeGenerator<ShipwreckType> typeGen;
		specificType = typeGen.getType();
		sprite.setTexture(GlobalTextureHandler::getInstance().getLandMassTextures().getShipwreckTextures().getTexture(std::get<ShipwreckType>(specificType)));
		sprite.setScale(5.f * scaling, 5.f * scaling);
		// For shipwreck loot pool, price limit is market price because no price is needed.
		loot = createLootPool(shipwreckLootPoolSize, marketPriceLimit, shipwreckItemLimit);
		break;
	}
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

std::vector<ShopItem> LandMass::createLootPool(int marketSize, int marketPrice, int marketItems) {
	std::set<std::string> itemNames = { 
		"Banana", "Cannonball", "Wood", "Gold", "Rum", "Cannon", "Sword", "Pistol", "Cloth", "Silk",
		"Spices", "Tea", "Coffee", "Tobacco", "Sugar", "Cotton", "Indigo", "Dye", "Salt", "Iron",
		"Copper", "Tin", "Lead", "Silver", "Gold", "Platinum", "Diamond", "Emerald", "Ruby", "Sapphire",
		"Pearl", "Amber", "Coral", "Ivory", "Jade", "Opal", "Quartz", "Topaz", "Turquoise", "Agate", "Garnet",
		"Lapis Lazuli", "Malachite", "Moonstone", "Obsidian", "Onyx", "Peridot", "Pyrite", "Rhodochrosite",
		"Sodalite", "Sunstone", "Tiger's Eye", "Tourmaline", "Zircon"
	};

	std::vector<ShopItem> generated;

	for (int i = 0; i < marketSize; i++) {
		// Randomly select an item that is not already in the market
		std::string randItem;
		do {
			randItem = *std::next(itemNames.begin(), rand() % itemNames.size());
		} while (std::find_if(market.begin(), market.end(), [&randItem](ShopItem& item) { return item.name == randItem; }) != market.end());

		// Randomly select the price and amount
		float randPrice = (rand() % marketPrice + 1) * 0.1f;
		int randAmount = rand() % marketItems + 1;

		// Create the item and add it to the market
		ShopItem item(randItem, randPrice, randAmount);
	}
	
	return generated;

}