#include "LandMass_PG.h"

using namespace PirateGame;

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
		} while (std::ranges::find_if(generated, [&randItem](const ShopItem& item) { return item.name == randItem; }) != generated.end());

		// Randomly select the price and amount
		const float randPrice = 0.1f * static_cast<float>(vm::randomValue(1, marketItems));
		const int randAmount = vm::randomValue(1, marketItems);

		// Create the item and add it to the market
		ShopItem item(randItem, randPrice, randAmount);
		generated.push_back(item);
	}
	
	return generated;
}