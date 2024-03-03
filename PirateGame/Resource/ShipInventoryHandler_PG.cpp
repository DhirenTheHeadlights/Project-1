#include "ShipInventoryHandler_PG.h"

using namespace PirateGame;

void ShipInventoryHandler::removeItemsFromInventory(ShopItem item) {
	for (auto& it : inventory) {
		if (it.name == item.name) {
			inventory.erase(std::remove(inventory.begin(), inventory.end(), it), inventory.end());
			break;
		}
	}
}

ShopItem& ShipInventoryHandler::getInventoryItem(std::string itemName) {
	for (auto& it : inventory) {
		if (it.name == itemName) {
			return it;
		}
	}

	// If the item is not found, return the first item in the inventory
	return inventory.front();
}
