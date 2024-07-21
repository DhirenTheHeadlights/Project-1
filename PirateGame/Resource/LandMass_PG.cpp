#include "LandMass_PG.h"

using namespace PirateGame;

std::vector<ShopItem> LandMass::createLootPool(int lootPoolSize, int lootPriceLimit, int numItems) {
	std::set<std::string> itemNames = { 
		"Banana", "Cannonball", "Wood", "Gold", "Rum", "Cannon", "Sword", "Pistol", "Cloth", "Silk",
		"Spices", "Tea", "Coffee", "Tobacco", "Sugar", "Cotton", "Indigo", "Dye", "Salt", "Iron",
		"Copper", "Tin", "Lead", "Silver", "Gold", "Platinum", "Diamond", "Emerald", "Ruby", "Sapphire",
		"Pearl", "Amber", "Coral", "Ivory", "Jade", "Opal", "Quartz", "Topaz", "Turquoise", "Agate", "Garnet",
		"Lapis Lazuli", "Malachite", "Moonstone", "Obsidian", "Onyx", "Peridot", "Pyrite", "Rhodochrosite",
		"Sodalite", "Sunstone", "Tiger's Eye", "Tourmaline", "Zircon"
	};

	std::vector<ShopItem> generated;

	for (int i = 0; i < lootPoolSize; i++) {
		// Randomly select an item that is not already in the market
		std::string randItem;
		do {
			randItem = *std::next(itemNames.begin(), rand() % itemNames.size());
		} while (std::find_if(generated.begin(), generated.end(), [&randItem](ShopItem& item) { return item.name == randItem; }) != generated.end());

		// Randomly select the price and amount
		float randPrice = (rand() % lootPriceLimit + 1) * 0.1f;
		int randAmount = rand() % numItems + 1;

		// Create the item and add it to the market
		ShopItem item(randItem, randPrice, randAmount);
		generated.push_back(item);
	}
	
	return generated;
}