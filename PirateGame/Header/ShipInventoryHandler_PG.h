#pragma once

/// This class contains the inventory for the ship. It will
/// also contain the loot/money the player has.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "ShopItem_PG.h"

namespace PirateGame {
	class ShipInventoryHandler {
	public:
		void addItemsToInventory(ShopItem item) { inventory.push_back(item); }
		void removeItemsFromInventory(ShopItem item);

		// Getters
		std::vector<ShopItem> getInventory() const { return inventory; }
		ShopItem& getInventoryItem(std::string itemName);
		float getGold() const { return gold; }

		// Setters
		void setGold(float gold) { this->gold = gold; }
		
	private:
		std::vector<ShopItem> inventory;

		// Money
		float gold = 0;
	};
}