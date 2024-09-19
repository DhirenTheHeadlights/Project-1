#pragma once

/// This class contains the inventory for the ship. It will
/// also contain the loot/money the player has.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "ShopItem_PG.h"

namespace PirateGame {
	class ShipInventoryHandler {
	public:
		ShipInventoryHandler() {
			/*addItemsToInventory(ShopItem("Test1", 10.f, 5));
			addItemsToInventory(ShopItem("Test2", 20.f, 10));
			addItemsToInventory(ShopItem("Test3", 30.f, 15));
			addItemsToInventory(ShopItem("Test4", 40.f, 20));
			addItemsToInventory(ShopItem("Test5", 50.f, 25));
			addItemsToInventory(ShopItem("Test6", 60.f, 30));
			addItemsToInventory(ShopItem("Test7", 70.f, 35));
			addItemsToInventory(ShopItem("Test8", 80.f, 40));
			addItemsToInventory(ShopItem("Test9", 90.f, 45));
			addItemsToInventory(ShopItem("Test10", 100.f, 50));
			addItemsToInventory(ShopItem("Test11", 110.f, 55));
			addItemsToInventory(ShopItem("Test12", 120.f, 60));
			addItemsToInventory(ShopItem("Test13", 130.f, 65));
			addItemsToInventory(ShopItem("Test14", 140.f, 70));
			addItemsToInventory(ShopItem("Test15", 150.f, 75));*/
		};

		void addItemsToInventory(ShopItem item) { inventory.push_back(item); }
		void removeItem(const ShopItem& item);

		// Getters
		std::vector<ShopItem>& getInventory() { return inventory; }
		ShopItem& getItem(const std::string& itemName);
		float getGold() const { return gold; }

		// Setters
		void setGold(float gold) { this->gold = gold; }
		void removeGold(float gold) { this->gold -= gold; }
		void addGold(float gold) { this->gold += gold; }

		void setInventory(std::vector<ShopItem> inventory) { this->inventory = inventory; }
		void setInventory(std::vector<std::pair<std::string, std::pair<float, int>>> playerInventory) {
			for (auto& item : playerInventory) {
				inventory.push_back(ShopItem(item.first, item.second.first, item.second.second));
			}
		}
		
	private:
		std::vector<ShopItem> inventory;

		// Money
		float gold = 0;
	};
}